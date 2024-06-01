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
    if (argc != 3) {
        fmt::println("Usage: {} <ip> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    
    struct sockaddr_in your_adr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = in_addr{inet_addr(argv[1])},
        .sin_zero = {0}
    };

    char msg1[] = "Hi!", msg2[] = "I'm another UDP host!", msg3[] = "Nice to meet you";
    sendto(sock, msg1, sizeof(msg1), 0,
        reinterpret_cast<sockaddr*>(&your_adr), sizeof(your_adr));
    sendto(sock, msg2, sizeof(msg2), 0,
        reinterpret_cast<sockaddr*>(&your_adr), sizeof(your_adr));
    sendto(sock, msg3, sizeof(msg3), 0,
        reinterpret_cast<sockaddr*>(&your_adr), sizeof(your_adr));
    close(sock);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
