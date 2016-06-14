#include "wrap.h"

int main(int argc, char **argv)
{
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;
	clientlen = sizeof(clientaddr);	

	if (argc != 2)
	{
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(1);
	}

	port = atoi(argv[1]);
	listenfd = Open_listenfd(port);
	for(;;)
	{
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		doit(connfd);
		close(connfd);
	}
}

int doit(int fd)
{
	char buf[MAXLINE];
	printf("Connect from TCP...\n");
	Rio_readinitb(&rio, fd);
	Rio_readlineb(&rio, buf, MAXLINE);
	Rio_writen(fd, "Sent from TCP server\n", strlen("Sent from TCP server\n"));
	Close(fd);
}
