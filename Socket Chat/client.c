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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Client must be exist.\n");
        exit(1);
    }

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    int client = atoi(argv[1]), client2;
    char mesaj[1024];

    if (clientSocket < 0)
    {
        perror("Failure to connect.");
        exit(1);
    }

    printf("Client socket created.\n");
    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Failure to connect.");
        exit(1);
    }

    printf("Connected to the server.\n");

    if (send(clientSocket, &client, sizeof(int), 0) < 0)
    {
        perror("Failure to send.");
        exit(1);
    }

    while (1)
    {
        printf("Kime mesaj göndermek istiyorsunuz: ");
        scanf("%d", &client2);
        printf("Mesajınız: ");
        memset(mesaj, 0, sizeof(mesaj));
        scanf("%[^\n]s", mesaj);
        mesaj[strlen(mesaj)-1] = '\0';

        if (send(clientSocket, &client2, sizeof(int), 0) < 0)
        {
            perror("Failure to send.");
            exit(1);
        }
        
        if (send(clientSocket, mesaj, sizeof(mesaj), 0) < 0)
        {
            perror("Failure to send.");
            exit(1);
        }

        if (strcmp(mesaj, "exit") == 0)
        {
            close(clientSocket);
            printf("Disconnected from the server.\n");
            exit(1);
        }
    }

    return 0;
}