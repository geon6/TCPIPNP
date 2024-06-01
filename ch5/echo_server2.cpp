#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include <iostream>

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

    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    struct  sockaddr_in server_address{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = in_addr{htonl(INADDR_ANY)},
        .sin_zero = {0}
    };

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }
    if (listen(server_socket, 2) == -1) {
        error_handling("listen() error");
    }
    
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    char buf[BUF_SIZE];
    for (int i = 0; i < 4; i++) {
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket == -1) {
            error_handling("accpet() error");
        } else {
            fmt::println("Connected client: {}", i + 1);
        }

        int msg_len = 0;
        while (msg_len = read(client_socket, buf, BUF_SIZE)) {
            if (msg_len == -1) {
                error_handling("read() error");
            }
            
            int w = write(client_socket, buf, msg_len);
            if (w == -1) {
                error_handling("write() error");
            }
        }
        close(client_socket);
    }
    close(server_socket);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
