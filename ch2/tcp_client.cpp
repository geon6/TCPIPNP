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
    if (argc != 3) {
        fmt::println("Usage: {} <ip> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in server_address{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = in_addr{inet_addr(argv[1])},
        .sin_zero = {0}
    };
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error_handling("connect() error");
    }

    int str_len = 0, idx = 0, read_len;
    std::string message(30, '\0');
    while (read_len = read(sock, &message[idx++], 1)) {
        if (read_len == -1) {
            error_handling("read() error");
        }
        str_len += read_len;
    }
    fmt::println("Message from server: {}", message);
    fmt::println("Fucntion read call count: {}", str_len);
    close(sock);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
