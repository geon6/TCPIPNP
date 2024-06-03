#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 30;

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in acpt_addr{
        .sin_family{AF_INET},
        .sin_port{htons(atoi(argv[1]))},
        .sin_addr{in_addr{htonl(INADDR_ANY)}}
    };

    if (bind(acpt_sock, (struct sockaddr*)&acpt_addr, sizeof(acpt_addr)) == -1) {
        error_handling("bind() error");
    }
    if (listen(acpt_sock, 5) == -1) {
        error_handling("listen() error");
    }

    struct sockaddr_in recv_addr;
    socklen_t recv_addr_len = sizeof(recv_addr);
    int recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_addr, &recv_addr_len);

    int str_len;
    char buf[BUF_SIZE];
    while (true) {
        str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
        if (str_len > 0) {
            break;
        }
    }
    buf[str_len] = 0;
    fmt::println("Buffering {} bytes", buf);

    str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
    buf[str_len] = 0;
    fmt::println("Read again: {}", buf);
    close(acpt_sock);
    close(recv_sock);
}
