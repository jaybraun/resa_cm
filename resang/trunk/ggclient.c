#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
/*
void main(int argc, char *argv[])
{
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    int rc;
    rc = tcp_client(argv[1], argv[2]);
}
*/

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int tcp_client_connect(char *host, char *port) /* int argc, char *argv[]) */
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
 
    portno = atoi(port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    return sockfd;
}

int tcp_client_write(int *sockfd, char *buffer, int *n)
{  
    *n = write(*sockfd,buffer,strlen(buffer));
//    if (n < 0) 
//         error("ERROR writing to socket");
    return 0;
}

int tcp_client_read(int *sockfd, char *buffer)
{
    int n;
    bzero(buffer,256);
    n = read(*sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    return n;
}

int tcp_client_close(int *sockfd)
{
    close(*sockfd);
    return 0;
}
