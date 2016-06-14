#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <sys/select.h>


#define LISTENQ 5
#define SERV_PORT_TCP 8000
#define SERV_PORT_UDP 8001
#define MAXLINE 1024

typedef struct sockaddr SA;
typedef void Sigfunc(int);

void str_echo_tcp(int sockfd);
void str_echo_udp(int sockfd);
Sigfunc * signal(int signo, Sigfunc *func);
Sigfunc * Signal(int signo, Sigfunc *func);
void sig_chld(int signo);
ssize_t	writen(int fd, const void *vptr, size_t n);
void Writen(int fd, void *ptr, size_t nbytes);
int max(int a, int b);
