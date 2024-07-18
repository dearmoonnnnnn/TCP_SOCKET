#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_PORT 8000

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}


int main(int argc, char *argv[])
{
    int sockfd;
    char buff[BUFSIZ];

    struct sockaddr_in serv_addr;           // 服务器地址结构
    serv_addr.sin_family = AF_INET;
    // serv_addr.sin_port = htons(SERV_PORT);       // 和服务器对应
    if (argc != 2){
        printf("usage: %s <port>\n", argv[0]);
        exit(1);
    }


    serv_addr.sin_port = htons(atoi(argv[1]));
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1){
        sys_err("socket error");
    }

    int ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
        sys_err("connect error");

    int i = 10;
    while(i--){
        write(sockfd, "hello, i am client \n", strlen("hello, i am client \n"));
        sleep(1);

        ret = read(sockfd, buff, sizeof(buff)); // ret 返回读取的字节数
        write(STDOUT_FILENO, buff, ret);   // 读取到屏幕上
    }

    close(sockfd);

    return 0;
}