#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string_view>
#include <string>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

void error_handling(std::string_view msg);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in server_address{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = in_addr{htonl(INADDR_ANY)},
        .sin_zero = {0}
    };
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }
    if (listen(server_socket, 5) == -1) {
        error_handling("listen() error");
    }

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
    if (client_socket == -1) {
        error_handling("accept() error");
    }

    std::string message{"Hello World"};
    write(client_socket, message.data(), message.size());
    close(server_socket);
    close(client_socket);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
