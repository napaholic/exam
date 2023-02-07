#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>

int sockfd, connfd;
unsigned int len;
struct sockaddr_in servaddr, cli;

#define ARR_MAX 65536
fd_set fds_read, fds_write, fds_asset;
int id, max_fd;
char *msgs[ARR_MAX];
int idx_cli[ARR_MAX];
char wbuf[42], rbuf[1025];

void fatal()
{
	write(2, "Fatal error\n", 12);
	close(sockfd);
	exit(1);
}

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;
	
	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;
	
	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void broadcast(int fd_from, char *msg)
{
	for (int fd = 0; fd < max_fd; ++fd) {
		if (FD_ISSET(fd, &fds_write) && fd != fd_from)
			send(fd, msg, strlen(msg), 0);
	}
}

void accept_cli(int fd_new)
{
	if (fd_new >= max_fd)
		max_fd = fd_new + 1;
	msgs[fd_new] = 0;
	idx_cli[fd_new] = id++;
	FD_SET(fd_new, &fds_asset);
	sprintf(wbuf, "server: client %d just arrived\n", idx_cli[fd_new]);
	broadcast(fd_new, wbuf);
}

void do_accept()
{
	len = sizeof(cli);
	connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (connfd < 0) {
		fatal();
	}
	else
		accept_cli(connfd);
}

void remove_cli(int fd_del)
{
	sprintf(wbuf, "server: client %d just left\n", idx_cli[fd_del]);
	broadcast(fd_del, wbuf);
	free(msgs[fd_del]);
	msgs[fd_del] = 0;
	FD_CLR(fd_del, &fds_asset);
	close(fd_del);
}

void send_msg(int fd_target, int end_msg)
{
	char *msg;
	
	rbuf[end_msg] = '\0';
	msgs[fd_target] = str_join(msgs[fd_target], rbuf);
	while (extract_message(&msgs[fd_target], &msg))
	{
		sprintf(wbuf, "client %d: ", idx_cli[fd_target]);
		broadcast(fd_target, wbuf);
		broadcast(fd_target, msg);
		free(msg);
		msg = 0;
	}
}

void do_recv(int fd_target)
{
	int cnt_read = recv(fd_target, rbuf, 1024, 0);
	if (cnt_read <= 0)
		remove_cli(fd_target);
	else
		send_msg(fd_target, cnt_read);
}

int main(int ac, char **av) {
	if (ac != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}
	
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		fatal();
	bzero(&servaddr, sizeof(servaddr));
	
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1]));
	
	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal();
	if (listen(sockfd, 0) != 0) {
		fatal();
	}
	
	max_fd = sockfd + 1;
	FD_ZERO(&fds_asset);
	FD_SET(sockfd, &fds_asset);
	
	while (1)
	{
		fds_read = fds_write = fds_asset;
		if (select(max_fd, &fds_read, &fds_write, 0, 0) < 0)
			fatal();
		for (int fd = 0; fd < max_fd; ++fd) {
			if (!FD_ISSET(fd, &fds_read))
				continue;
			if (fd == sockfd)
				do_accept();
			else
				do_recv(fd);
		}
	}
	return (0);
}