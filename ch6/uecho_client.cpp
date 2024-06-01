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
    
    struct sockaddr_in server_address{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = in_addr{inet_addr(argv[1])},
        .sin_zero = {0}
    };

    char message[BUF_SIZE];
    struct sockaddr_in from_address;
    socklen_t addr_size;
    int str_len;
    while (true) {
        fmt::print("Insert message(q to quit): ");
        fgets(message, sizeof(message), stdin);
        if (strcmp(message, "q\n") == 0 || strcmp(message, "Q\n") == 0) {
            break;
        }
        sendto(sock, message, strlen(message), 0, 
            reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
        addr_size = sizeof(from_address);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, 
            reinterpret_cast<sockaddr*>(&from_address), &addr_size);
        message[str_len] = 0;
        fmt::print("Message from server: {}", message);
    }
    close(sock);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
