#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <string_view>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 1024;

void error_handling(std::string_view msg);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    // socket bind listen accpet
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }
    
    struct sockaddr_in serv_addr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = in_addr{htonl(INADDR_ANY)},
        .sin_zero = {0}
    };
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }
    
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int str_len;
    char buf[BUF_SIZE];
    for (int i = 0; i < 5; i++) {
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
        if (clnt_sock == -1) {
            error_handling("accpet() error");
        } else {
            fmt::println("Connected client {}", i + 1);
        }

        while ((str_len = read(clnt_sock, buf, BUF_SIZE))) {
            if (str_len == -1) {
                error_handling("read() error");
            }
            write(clnt_sock, buf, str_len);
        }
        close(clnt_sock);
    }
    close(serv_sock);
    exit(EXIT_SUCCESS);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}