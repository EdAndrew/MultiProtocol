#include "server.h"

int main(int argc, char **argv)
{
	int					listenfdTcp, connfd, listenfdUdp;
	int					maxfdp1;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddrTcp, servaddrUdp;
	void				sig_chld(int);
	fd_set				rset;

	listenfdTcp = socket(AF_INET, SOCK_STREAM, 0);
	listenfdUdp = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddrTcp, sizeof(servaddrTcp));
	servaddrTcp.sin_family      = AF_INET;
	servaddrTcp.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddrTcp.sin_port        = htons(8000);

	bzero(&servaddrUdp, sizeof(servaddrUdp));
	servaddrUdp.sin_family      = AF_INET;
	servaddrUdp.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddrUdp.sin_port        = htons(8001);

	bind(listenfdTcp, (SA *) &servaddrTcp, sizeof(servaddrTcp));
	bind(listenfdUdp, (SA *) &servaddrUdp, sizeof(servaddrUdp));

	listen(listenfdTcp, LISTENQ);
	Signal(SIGCHLD, sig_chld);	/* must call waitpid() */

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		FD_ZERO(&rset);
		FD_SET(listenfdTcp, &rset);
		FD_SET(listenfdUdp, &rset);
		maxfdp1 = max(listenfdTcp, listenfdUdp) + 1;
		select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfdTcp, &rset))
		{
			if ( (connfd = accept(listenfdTcp, (SA *) &cliaddr, &clilen)) < 0)
			{
				if (errno == EINTR)
					continue;		/* back to for() */
				else
					fprintf(stderr, "accept error");
			}
			if ( (childpid = fork()) == 0) /* child process */
			{
				close(listenfdTcp);	/* close listening socket */
				str_echo_tcp(connfd);	/* process the request */
				exit(0);
			}
			close(connfd);			/* parent closes connected socket */
		}
		else if (FD_ISSET(listenfdUdp, &rset))
		{
			if ( (childpid = fork()) == 0) /* child process */
			{
				str_echo_udp(listenfdUdp);	/* process the request */
				exit(0);
			}
		}
	}
}
