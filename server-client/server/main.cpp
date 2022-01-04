//
// Created by ubuntu on 03/01/2022.
//

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

int main(int argc, char* argv[])
{
    int sockfd, newsockfd, portno = 8080;
    struct sockaddr_in serv_addr, cli_addr;
    int serv_addrlen = sizeof (serv_addr);
    int cli_addrlen = sizeof (cli_addr);
    char buffer[256] = {0};
    char msg[] = "Hello from server!";

    //creating socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
    {
        perror("Fail to create socket");
        exit(EXIT_FAILURE);
    }

    //socket -> bind -> listen -> accept -> send/receive
    //socket -> connect -> send/receive

    bzero((char*) &serv_addr, serv_addrlen);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr*) &serv_addr, serv_addrlen) < 0)
    {
        perror("ERROR binding server address");
    }

    listen(sockfd, 5);

    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, reinterpret_cast<socklen_t *>(&cli_addrlen));

    if(newsockfd < 0)
    {
        perror("ERROR create new socket");
        exit(EXIT_FAILURE);
    }

    char client_ip_addr[256];
    bzero(client_ip_addr, 256);
    inet_ntop(AF_INET, &cli_addr.sin_addr, buffer, sizeof client_ip_addr);
    printf("Got connection from %u on port %d\n", client_ip_addr, portno);

    send(newsockfd, msg, strlen(msg), 0);

    bzero(buffer, 256);
    int number_of_bytes_read = recv(newsockfd, buffer, 255, 0);

    if(number_of_bytes_read<0)
    {
        perror("ERROR cannot read from socket");
    }

    printf("Client: %s", buffer);

    close(newsockfd);
    close(sockfd);

    return 0;
}