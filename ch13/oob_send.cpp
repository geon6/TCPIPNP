#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string_view>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fmt::println("Usage: {} <ip> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr{
        .sin_family{AF_INET},
        .sin_port{htons(atoi(argv[2]))},
        .sin_addr{in_addr{inet_addr(argv[1])}}
    };

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        error_handling("connect() error");
    }

    write(sock, "123", strlen("123"));
    send(sock, "4", strlen("4"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    close(sock);
}