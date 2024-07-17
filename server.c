#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>

// @descripiton: 处理系统调用中的错误
void sys_err(const char *str){
    perror(str);
    exit(1);
}

int main(int argc, char *argv[]){
    int sockfd, newfd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t clit_addr_len;

    int ret;                // 表示实际读取的字节数，ret == 0 表示读取完毕
    char buf[BUFSIZ];       // 缓冲区，用于存储读取的客户端数据，BUFSIZ 是系统定义好的宏，大小为4096


    if(argc != 2){
        printf("usage: %s <port>\n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1){
        sys_err("socket error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    // serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
        sys_err("bind error");
    }

    listen(sockfd, 128);

    // 因为 len 是传入传出参数， 不能直接 sizeof 获取， 需要先赋值
    clit_addr_len = sizeof(client_addr);
    newfd = accept(sockfd, (struct sockaddr *) &client_addr, &clit_addr_len);

    if(newfd == -1){
        sys_err("socket error");
    }

    while(1){
        ret = read(newfd, buf, sizeof(buf));
        if(ret == -1){
            sys_err("read error");
        }

        // 对读取的字符进行处理
        for(int i = 0; i < ret; i++){
            buf[i] = toupper(buf[i]);
        }

        write(newfd, buf, ret);
    }


    close(sockfd);
    close(newfd);
}
