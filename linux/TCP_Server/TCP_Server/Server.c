//
//  Server.c
//  Socket-API
//
//  Created by YangHan on 2018/3/6.
//  Copyright © 2018年 YangHan. All rights reserved.
//

#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

#define SERVER_PORT 8000
#define MAX_BUF_SIZE   4096

int main() {
    
    // socket
    struct sockaddr_in servaddr, clienaddr;
    bzero(&servaddr, sizeof(servaddr));
    bzero(&clienaddr, sizeof(clienaddr));
    
    // 创建一个 socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    printf("服务器端 socket fd : %zd\n", sockfd);
    
    // bind
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    
    // listen
    listen(sockfd, 128);
    
    // accept
    char ipstr[128];
    char buf[MAX_BUF_SIZE];
    while (1) {
        int addrlen = sizeof(clienaddr);
        
        // 阻塞直到客户端连接
        int confd = accept(sockfd,
                           (struct sockaddr *)&clienaddr,
                           (socklen_t *)&addrlen);
        
        printf("服务器端 accept 成功后返回的 confd : %zd\n", confd);
        
        // 将 IP 地址的 numeric 换成 presentation
        inet_ntop(AF_INET,
                  &clienaddr.sin_addr.s_addr,
                  ipstr,
                  sizeof(ipstr));
        
        // 客户端端口号
        int port = ntohs(clienaddr.sin_port);
        
        // 打印请求客户端的 IP 地址和 端口号
        printf("client ip : %s  port : %zd\n", ipstr, port);
        
        // 处理客户端提交的数据
        ssize_t len = read(confd, buf, sizeof(buf));
        printf("服务器读取到数据长度为 %zd\n", len);
        if (len > 0) {
            for (int i = 0; i < len; i++) {
                buf[i] = toupper(buf[i]);
            }
            
            // 将处理后数据返回客户端
            write(confd, buf, sizeof(buf));
        }
        
    
        // 关闭 socket
        close(confd);
    }
    close(sockfd);
    
    return 0;
}