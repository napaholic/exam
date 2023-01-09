#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/select.h>
#include <stdlib.h>

int sockfd, connfd;
unsigned int len;
struct sockaddr_in servaddr, cli;

#define ARR_MAX 65536
fd_set fds_write, fds_read, fds_asset;
int id, max_fd;
char* msgs[ARR_MAX];
int idx_cli[ARR_MAX];
char wbuf[42], rbuf[1025];

void fatal_err()
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


int main(int ac, char **av) {
	if (ac != 2) {
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}
	
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		fatal_err();
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));
	
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1]));
	
	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
		fatal_err();
	}
	else
		printf("Socket successfully binded..\n");
	if (listen(sockfd, 0) != 0) {
		fatal_err();
	}
	
	max_fd = sockfd + 1;
	FD_ZERO(&fds_asset);
	FD_SET(sockfd, &fds_asset);

	while(1)
	{
		fds_read = fds_write = fds_asset;
		if (select(max_fd, &fds_read, &fds_write, 0, 0) < 0)
			fatal_err();
	}
	len = sizeof(cli);
	connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");
}