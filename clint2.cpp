#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sys/types.h>
#include <string.h>
#include <thread>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 100


void recive_massage(SOCKET sock, char buffer[BUFFER_SIZE])
{
    while (1)
    {
        if (recv(sock, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR)
        {
            return;
        }
        else
        {
            printf("%s\n", buffer);
        }
    }
}

int main()
{

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        // printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }
    else
    {
        // printf("WSAStartup success\n");
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET)
    {
        // printf("socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    else
    {
        // printf("socket success\n");
    }

    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        // printf("connect failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    else
    {
        // printf("connect success\n");
    }

    char buffer[BUFFER_SIZE];

    char massage[BUFFER_SIZE];
    char name[] = ":Amir";

    std::thread t;
    t = std::thread(recive_massage, sock, buffer);

    while (1)
    {

        fgets(massage, BUFFER_SIZE, stdin);
        if (massage[strlen(massage) - 1] == '\n')
        {
            massage[strlen(massage) - 1] = '\0';
        }
        strcat(massage, name);

        if (send(sock, massage, strlen(massage), 0) == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
        }
        else
        {
            // printf("send success\n");
            // return 0;
        }
    }

    printf("closing connection\n");

    closesocket(sock);

    WSACleanup();

    return 0;
}
