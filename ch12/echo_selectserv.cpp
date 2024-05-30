#include <cstdio>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(const char* msg);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }

    if (listen(server_socket, 5) == -1) {
        error_handling("listen() error");
    }

    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(server_socket, &read_set);

    int fd_max = server_socket;
    char buf[BUF_SIZE];

    while (true) {
        fd_set copy = read_set;
        struct timeval timeout{.tv_sec = 5, .tv_usec = 5000};
        int fd = select(fd_max + 1, &copy, NULL, NULL, &timeout);
        if (fd == -1) {
            error_handling("select() error");
        }
        if (fd == 0) {
            continue;
        }

        for (int i = 0; i < fd_max + 1; i++) {
            if (!FD_ISSET(i, &copy)) {
                continue;
            }
            if (i == server_socket) { // new connection
                struct sockaddr_in client_address;
                socklen_t client_address_len = sizeof(client_address);
                int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
                if (client_socket == -1) {
                    error_handling("accept() error");
                }
                FD_SET(client_socket, &read_set);
                fd_max = std::max(fd_max, client_socket);
                std::cout << "new client: " << client_socket << std::endl;
            } else {
                int len = read(i, buf, BUF_SIZE);
                if (len == 0) {
                    FD_CLR(i, &read_set);
                    close(i);
                    std::cout << "close client: " << i << std::endl;
                } else {
                    write(i, buf, len);
                }
            }
        }
    }
    close(server_socket);
}

void error_handling(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}