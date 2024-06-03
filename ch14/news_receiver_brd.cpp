#include <cstdlib>
#include <string_view>
#include <string>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 30;

void error_handling(std::string_view msg);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr{
        .sin_family{AF_INET},
        .sin_port{htons(atoi(argv[1]))},
        .sin_addr{in_addr{htonl(INADDR_ANY)}}
    };

    if (bind(recv_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        error_handling("bind() error");
    }

    std::string str(BUF_SIZE, '\0');
    while (true) {
        int str_len = recvfrom(recv_sock, str.data(), str.size() - 1, 0, NULL, 0);
        if (str_len < 0) {
            break;
        }
        str[str_len] = '\0';
        fmt::println(str);
    }
    close(recv_sock);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
