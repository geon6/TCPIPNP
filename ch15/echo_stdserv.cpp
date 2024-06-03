#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <string_view>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 1024;

void error_handling(std::string_view msg);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in serv_addr{
        .sin_family{AF_INET},
        .sin_port{htons(atoi(argv[1]))},
        .sin_addr{in_addr{htonl(INADDR_ANY)}}
    };

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    struct sockaddr_in clnt_addr;
    socklen_t client_address_len = sizeof(clnt_addr);
    char message[BUF_SIZE];

    for (int i = 0; i < 5; i++) {
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &client_address_len);
        if (clnt_sock == -1) {
            error_handling("accept() error");
        } else {
            fmt::println("Connected client: {}", i + 1);
        }

        FILE* readfp = fdopen(clnt_sock, "r");
        FILE* writefp = fdopen(clnt_sock, "w");
        while (!feof(readfp)) {
            fgets(message, BUF_SIZE, readfp);
            fputs(message, writefp);
            fflush(writefp);
        }
        fclose(readfp);
        fclose(writefp);
    }

    close(serv_sock);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
