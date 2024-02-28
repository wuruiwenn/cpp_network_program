#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

void error_handling(char* msg);
int main(int argc, char* argv[])
{
    //客户端流程：
    // socket -> connect -> send/receive_msg -> close socket
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    //1.创建客户端socket
    int client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        error_handling("client socket() error !");
    }

    //2.client端利用connect向server端发送连接请求
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);//通过gcc编译参数传入
    server_address.sin_port = htons(atoi(argv[2]));
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
    {
        error_handling("client connect() error !");
    }

    //向server端发送数据
    write(client_socket, "hello world. from client", sizeof("hello world. from client"));
    close(client_socket);
}

void error_handling(char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}