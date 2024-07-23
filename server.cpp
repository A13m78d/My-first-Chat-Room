#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sys/types.h>
#include <string.h>
#include <thread>

#define SERVER_IP "127.0.0.1"

#define SERVER_PORT 12345
#define MAX_CLINTS 10
#define BUFFER_SIZE 100

SOCKET clientSock[MAX_CLINTS] = {0};
WSADATA wsaData;

void reciving_buffer(SOCKET clientSock, SOCKET sock);
void sending_massage(SOCKET clientSock, SOCKET sock, char message[BUFFER_SIZE]);

void sending_massage_toAll(SOCKET me, SOCKET sock, char message[BUFFER_SIZE])
{

    for (int i = 0; clientSock[i] != 0; i++)
    {
        if (clientSock[i] == me){
            continue;
        }

        if (send(clientSock[i], message, BUFFER_SIZE, 0) == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
        }
    }
}

void sending_massage(SOCKET clientSock, SOCKET sock, char message[BUFFER_SIZE])
{

    if (send(clientSock, message, BUFFER_SIZE, 0) == SOCKET_ERROR)
    {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
    }
    reciving_buffer(clientSock, sock);
}

void reciving_buffer(SOCKET clientSock, SOCKET sock)
{
    while (1)
    {
        char buffer[BUFFER_SIZE];
        char to_send[BUFFER_SIZE];
        if (recv(clientSock, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR)
        {
            printf("recv failed  with error: %d\n", WSAGetLastError());
            return;
        }
        else
        {
            char name[20];
            for (int i = 0; i < strlen(buffer); i++)
            {
                if (buffer[i] == ':')
                {
                    char *x = &(buffer[i + 1]);
                    strcat(name, x);
                    memset(x - 1, 0, strlen(name));
                }
            }

            printf("%s : %s\n", name, buffer);
            strcat(name, " :");
            strcat(name, buffer);

            strcpy(to_send, name);

            memset(name, 0, strlen(name));
            memset(buffer, 0, strlen(buffer));

        }
        sending_massage_toAll(clientSock, sock, to_send);
    }
}

int main()
{

    

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
    }
    else
    {
        printf("WSAStartup success\n");
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET)
    {
        printf("socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    else
    {
        printf("socket success\n");
    }

    struct sockaddr_in serverAddr = {0};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    else
    {
        printf("bind success\n");
    }

    if (listen(sock, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Server listening on port %d\n", SERVER_PORT);
    printf("Waiting for client...\n");

    struct sockaddr_in clientAddr[MAX_CLINTS] = {0};
    int clientAddrSize[MAX_CLINTS] = {0};
    std::thread thread[MAX_CLINTS];

    for (int i = 0; i < MAX_CLINTS; i++)
    {
        clientAddrSize[i] = sizeof(clientAddr[i]);

        clientSock[i] = accept(sock, (struct sockaddr *)&clientAddr[i], &clientAddrSize[i]);

        if (clientSock[i] == INVALID_SOCKET)
        {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        printf("Client connected: %s:%d\n", inet_ntoa(clientAddr[i].sin_addr), ntohs(clientAddr[i].sin_port));

        char message[BUFFER_SIZE] = "Hi Lets Chat...";
        thread[i] = std::thread(sending_massage, clientSock[i], sock, message);
    }

    closesocket(sock);

    WSACleanup();
    return 0;
}
