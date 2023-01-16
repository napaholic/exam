#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct s_server
{
    int server_socket;
    struct sockaddr_in server_address;

    fd_set sockets;
    fd_set read_set;
    fd_set write_set;

    int max_fds;
    int max_id;
} t_server;

typedef struct s_client
{
    int fd;
    int id;
    char *message_buffer;
    struct s_client *next;
} t_client;

void fatalError()
{
    write(2, "Fatal error\n", strlen("Fatal error\n"));
    exit(1);
}

char *str_join(char *buf, char *add)
{
    char *newbuf;
    int len;

    if (buf == 0)
        len = 0;
    else
        len = strlen(buf);
    newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
    if (newbuf == 0) return (0);
    newbuf[0] = 0;
    if (buf != 0) strcat(newbuf, buf);
    free(buf);
    strcat(newbuf, add);
    return (newbuf);
}

void sendToAllClients(t_server *server, t_client *client, char *text, int fd)
{
    while (client != 0)
    {
        if (client->fd != fd && FD_ISSET(client->fd, &server->write_set))
            send(client->fd, text, strlen(text), MSG_DONTWAIT);
        client = client->next;
    }
}

t_client *clientDisconnection(t_server *server, t_client *start,
                              t_client *connected_socket)
{
    t_client *it;
    t_client *prior;
    char buffer[100];

    it = start;
    prior = 0;
    sprintf(buffer, "server: client %d just left\n", connected_socket->id);
    sendToAllClients(server, start, buffer, connected_socket->fd);
    while (it != 0)
    {
        if (it->fd == connected_socket->fd)
        {
            if (prior == 0)
                start = start->next;
            else
                prior->next = prior->next->next;
            break;
        }
        prior = it;
        it = it->next;
    }
    FD_CLR(connected_socket->fd, &server->sockets);
    close(connected_socket->fd);
    free(connected_socket->message_buffer);
    free(connected_socket);
    return (start);
}

t_client *clientMessage(t_server *server, t_client *start,
                        t_client *connected_socket)
{
    char message[1000000];
    char line[1000000];
    char buffer[1000100];
    int len;
    int i;
    int j;

    i = 0;
    j = 0;
    len =
        recv(connected_socket->fd, message, sizeof(message) - 1, MSG_DONTWAIT);
    if (len == 0)
        start = clientDisconnection(server, start, connected_socket);
    else if (len == -1)
        return (start);
    else
    {
        message[len] = 0;
        while (message[i])
        {
            line[j] = message[i];
            if (message[i] == '\n')
            {
                message[len + 1] = 0;
                if (connected_socket->message_buffer != 0)
                {
                    sprintf(buffer, "client %d: %s%s", connected_socket->id,
                            connected_socket->message_buffer, line);
                    free(connected_socket->message_buffer);
                    connected_socket->message_buffer = 0;
                }
                else
                    sprintf(buffer, "client %d: %s", connected_socket->id,
                            line);
                sendToAllClients(server, start, buffer, connected_socket->fd);
                j = -1;
            }
            i++;
            j++;
        }
        if (message[i - 1] != '\n')
        {
            line[j] = 0;
            connected_socket->message_buffer =
                str_join(connected_socket->message_buffer, line);
            if (connected_socket->message_buffer == 0) fatalError();
        }
    }
    return (start);
}

t_client *clientConnection(t_server *server, t_client *client)
{
    int new_connection;
    char buffer[100];
    t_client *new;
    t_client *it;
    socklen_t len;

    it = client;
    len = sizeof(server->server_address);
    new_connection = accept(server->server_socket,
                            (struct sockaddr *)&server->server_address, &len);
    if (new_connection < 0) fatalError();
    if (new_connection > server->max_fds) server->max_fds = new_connection;
    FD_SET(new_connection, &server->sockets);
    if ((new = malloc(sizeof(client))) == 0) fatalError();
    new->fd = new_connection;
    new->id = server->max_id++;
    new->message_buffer = 0;
    new->next = 0;
    if (client == 0)
        client = new;
    else
    {
        while (it != 0) it = it->next;
        it->next = new;
    }
    sprintf(buffer, "server: client %d just arrived\n", new->id);
    sendToAllClients(server, client, buffer, new_connection);
    return (client);
}

void launchServer(t_server server)
{
    t_client *client;
    t_client *it;

    client = 0;
    server.max_fds = server.server_socket;
    server.max_id = 0;
    FD_ZERO(&server.sockets);
    FD_SET(server.server_socket, &server.sockets);
    while (1)
    {
        server.read_set = server.sockets;
        server.write_set = server.sockets;
        it = client;
        if (select(server.max_fds + 1, &server.read_set, &server.write_set, 0,
                   0) == -1)
            fatalError();
        if (FD_ISSET(server.server_socket, &server.read_set))
        {
            client = clientConnection(&server, client);
            continue;
        }
        while (it)
        {
            if (FD_ISSET(it->fd, &server.read_set))
                client = clientMessage(&server, client, it);
            it = it->next;
        }
    }
}

void setupServer(t_server *server, int port)
{
    server->server_address.sin_family = AF_INET;
    server->server_address.sin_addr.s_addr = htonl(2130706433);
    server->server_address.sin_port = htons(port);

    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_socket == -1) fatalError();
    if ((bind(server->server_socket,
              (const struct sockaddr *)&server->server_address,
              sizeof(server->server_address))) != 0)
        fatalError();
    if (listen(server->server_socket, 0) != 0) fatalError();
}

int main(int argc, char **argv)
{
    t_server server;

    if (argc != 2)
    {
        write(2, "Wrong number of arguments\n",
              strlen("Wrong number of arguments\n"));
        exit(1);
    }
    setupServer(&server, atoi(argv[1]));
    launchServer(server);
}
