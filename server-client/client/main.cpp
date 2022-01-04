//
// Created by ubuntu on 03/01/2022.
//

#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr;
    int portno = 8080;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        perror("ERROR create socket");
        exit(EXIT_FAILURE);
    }

    bzero((char*)&serv_addr, sizeof serv_addr);

    struct hostent* server = gethostbyname("localhost");
    bcopy((char*)server->h_addr_list[0], (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_family = AF_INET;

    if(connect(sockfd, (struct sockaddr* )&serv_addr, sizeof serv_addr) == -1)
    {
        perror("ERROR connection");
        exit(EXIT_FAILURE);
    }

    printf("Connected to %s on port %d\n", server->h_addr_list[0], portno);

    bzero(buffer, 256);
    printf("Enter your message: ");
    fgets(buffer, 255, stdin);

    int number_of_bytes_sent = send(sockfd, buffer, strlen(buffer), 0);

    if(number_of_bytes_sent < 0)
    {
        perror("ERROR sending to server");
        exit(EXIT_FAILURE);
    }

    bzero(buffer, 256);
    int number_of_bytes_read = recv(sockfd, buffer, 255, 0);

    if(number_of_bytes_read < 0)
    {
        perror("ERROR reading from server");
        exit(EXIT_FAILURE);
    }

    printf("Server: %s\n", buffer);

    close(sockfd);
}