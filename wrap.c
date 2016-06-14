#include "wrap.h"

void str_echo_tcp(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
    write(sockfd, "From tcp server.\n", strlen("From tcp server.\n"));
	while ( (n = read(sockfd, buf, MAXLINE)) > 0)
	Writen(sockfd, buf, n);

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		fprintf(stderr, "str_echo_tcp: read error");
}

void str_echo_udp(int sockfd)
{
    ssize_t		               n;
	char		               buf[MAXLINE];
    struct sockaddr_in         cliaddr;
    socklen_t                  len;

    len = sizeof(cliaddr);
    n = recvfrom(sockfd, buf, MAXLINE, 0, (SA *)&cliaddr, &len);
    sendto(sockfd, buf, n, 0, (SA *)&cliaddr, len);
    sendto(sockfd, "From udp server.\n", strlen("From udp server.\n"), 0, (SA *)&cliaddr, len);
}

Sigfunc * signal(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}
/* end signal */

Sigfunc * Signal(int signo, Sigfunc *func)	/* for our signal() function */
{
	Sigfunc	*sigfunc;

	if ( (sigfunc = signal(signo, func)) == SIG_ERR)
		fprintf(stderr, "signal error");
	return(sigfunc);
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    pid = wait(&stat);
    printf("Client close TCP connect, server child %d terminated\n", pid);
    return;
}

ssize_t	writen(int fd, const void *vptr, size_t n) /* Write "n" bytes to a descriptor. */
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}
/* end writen */

void Writen(int fd, void *ptr, size_t nbytes)
{
	if (writen(fd, ptr, nbytes) != nbytes)
		fprintf(stderr, "writen error");
}

int max(int a, int b)
{
    return a > b ? a : b;
}
