#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1983

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0), newsockfd;
    socklen_t addr_size;
    struct sockaddr_in serverAddr, newServerAddr;

    if (sockfd < 0)
    {
        perror("Failure to connect.");
        exit(1);
    }
    
    printf("Server socket created.\n");
    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char mesaj[1000];

    if (bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Failure to bind.");
        exit(1);
    }

    printf("Bind to port: %d\n", PORT);

    if (listen(sockfd, 10) == 0)
        printf("Listening...\n");
    else
    {
        perror("Failure to listen.");
        exit(1);
    }

    int client, client2;

    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &newServerAddr, &addr_size);

        if (newsockfd < 0)
        {
            perror("Failure to connect.");
            close(newsockfd);
            exit(1);
        }

        if (recv(newsockfd, &client, sizeof(int), 0) < 0)
        {
            perror("Failure to recieve");
            exit(1);
        }

        printf("Connection accepted from %s:%d which has ID #%d.\n", inet_ntoa(newServerAddr.sin_addr), ntohs(newServerAddr.sin_port), client);

        pid_t childpid = fork();

        if (!childpid)
        {
            while (1)
            {
                memset(mesaj, 0, sizeof(mesaj));

                if (recv(newsockfd, &client2, sizeof(int), 0) < 0)
                {
                    printf("Failure to recieve.\n");
                    exit(1);
                }

                if (recv(newsockfd, mesaj, sizeof(mesaj), 0) < 0)
                {
                    printf("Failure to recieve.\n");
                    exit(1);
                }

                if (strcmp(mesaj, "exit") == 0)
                {
                    printf("Disconnected from %s:%d.\n", inet_ntoa(newServerAddr.sin_addr), ntohs(newServerAddr.sin_port));
                    break;
                }
                else
                    printf("Client #%d -> Client #%d\t: %s\n", client, client2, mesaj);
            }
        }
    }

    close(sockfd);

    return 0;
}