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

#define BUF_SIZE 30

void error_handling(std::string_view msg);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        error_handling("UDP socket creation error");
    }
    
    struct sockaddr_in server_address{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = in_addr{htonl(INADDR_ANY)},
        .sin_zero = {0}
    };

    if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }
    
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    int str_len;
    char message[BUF_SIZE];
    while (true) {
        client_address_size = sizeof(client_address);
        str_len = recvfrom(server_socket, message, BUF_SIZE, 0,
            reinterpret_cast<sockaddr*>(&client_address), &client_address_size);
        sendto(server_socket, message, str_len, 0,
            reinterpret_cast<sockaddr*>(&client_address), client_address_size);
    }
    close(server_socket);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
