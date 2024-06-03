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

constexpr int BUF_SIZE = 100;

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
    struct sockaddr_in send_addr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = in_addr{inet_addr(argv[1])},
        .sin_zero = {0}
    };

    if (connect(sock, (struct sockaddr*)&send_addr, sizeof(send_addr)) == -1) {
        error_handling("connect() error");
    }

    write(sock, "123", strlen("123"));
    close(sock);
}
