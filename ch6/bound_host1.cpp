#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string_view>

#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 30;

void error_handling(std::string_view msg);

int main(int argc, char* argv[]) {

    if (argc == 2) {
        fmt::println("Usage: {} <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    
    struct sockaddr_in my_adr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = in_addr{htonl(INADDR_ANY)},
        .sin_zero = {0}
    };

    if (bind(sock, reinterpret_cast<sockaddr*>(&my_adr), sizeof(my_adr)) == -1) {
        error_handling("bind() error");
    }

    struct sockaddr_in your_adr;
    char message[BUF_SIZE];
    socklen_t adr_sz;
    int str_len;
    for (int i = 0; i < 3; i++) {
        sleep(5);
        adr_sz = sizeof(your_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, 
            reinterpret_cast<sockaddr*>(&your_adr), &adr_sz);
        fmt::println("Message: {}: {}", i + 1, message);
    }
    close(sock);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
