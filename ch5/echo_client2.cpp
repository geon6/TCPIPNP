#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 1024;

void error_handling(std::string_view msg);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fmt::println("Usage: {} <ip> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in serv_addr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = in_addr{inet_addr(argv[1])},
        .sin_zero = {0}
    };

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    } else {
        fmt::println("Connected......");
    }
    
    std::string str;
    str.reserve(BUF_SIZE);
    while (true) {
        fmt::println("Input message(Q to quit): ");
        std::getline(std::cin, str);
        if (str == "Q" || str == "q") {
            break;
        }
        
        int str_len = write(sock, str.data(), str.size());
        int recv_len = 0;
        while (recv_len < str_len) {
            int recv_cnt = read(sock, &str[recv_len], BUF_SIZE - 1);
            if (recv_cnt == -1) {
                error_handling("read() error");
            }
            recv_len += recv_cnt;
        }
        str[recv_len] = '\0';
        fmt::println("Message from server: {}", str);
    }
    close(sock);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
