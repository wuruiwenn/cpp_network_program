#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/socket.h>
#include <arpa/inet.h>

#define SERVER_TCP_PORT 8888
#define MAX_SIZE 80
void error_handling(char* msg);
int main(int argc, char* argv[]) {
    //服务端流程：
    // socket -> bind -> listen -> accept -> send/receive_msg -> close socket

    //客户端流程：
    // socket -> connect -> send/receive_msg -> close socket

    //1.创建 服务端 socket
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        error_handling("server socket() error !");
    }

    //2.使用bind将服务端的socket和ip+端口号绑定在一起
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    // server_address.sin_port = htons(atoi(argv[1])); //atoi：字符串转为整型
    server_address.sin_port = htons(SERVER_TCP_PORT);
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
    {
        error_handling("server bind() error !");
    }

    //3.使用listen将server端的socket转为被动接收状态
    if (listen(server_socket, 5) == -1)
    {
        error_handling("listen() error !");
    }
    printf("server listening...\n");

    //4.server端调用accept，主动受理客户端发来的连接请求
    struct sockaddr_in client_protocal_addr;
    socklen_t client_protocal_addr_size = sizeof(client_protocal_addr);
    //accept返回一个已经连接的socket的描述字，表征了一次connection
    int connected_sock_desc = accept(
        server_socket,
        (struct sockaddr*)&client_protocal_addr,
        &client_protocal_addr_size);
    if (connected_sock_desc == -1)
    {
        error_handling("accept() error !");
    }
    //读取client端发来的数据
    char serv_read_ret_str[MAX_SIZE];
    int server_read_len = read(connected_sock_desc, serv_read_ret_str, MAX_SIZE);
    serv_read_ret_str[server_read_len] = 0;
    printf("server received data: %s\n", serv_read_ret_str);
    close(connected_sock_desc);
}

void error_handling(char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}