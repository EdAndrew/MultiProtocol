#include "server.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen);

int main(int argc, int *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./client_udp <IP_address>\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8001);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));
}

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1], recvline1[MAXLINE + 1];

	fgets(sendline, MAXLINE, fp);
	sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
	n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
	recvline[n] = 0;	/* null terminate */
    n = recvfrom(sockfd, recvline1, MAXLINE, 0, NULL, NULL);
    recvline1[n] = 0;
	fputs(recvline, stdout);
    fputs(recvline1, stdout);
}
