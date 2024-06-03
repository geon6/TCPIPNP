#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <deque>
#include <string_view>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 30;

void error_handling(std::string_view msg);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sigaction act;
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in recv_addr{
        .sin_family{AF_INET},
        .sin_port{htons(atoi(argv[1]))},
        .sin_addr{in_addr{htonl(INADDR_ANY)}}
    };

    if (bind(acpt_sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(acpt_sock, 5) == -1) {
        error_handling("listen() error");
    }

    struct sockaddr_in serv_addr;
    socklen_t serv_addr_len = sizeof(serv_addr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_addr, &serv_addr_len);

    fcntl(recv_sock, F_SETOWN, getpid());
    int state = sigaction(SIGURG, &act, NULL);
    int str_len;
    char buf[BUF_SIZE];
    while ((str_len = read(recv_sock, buf, BUF_SIZE)) != 0) {
        if (str_len == -1) {
            continue;
        }

        buf[str_len] = 0;
        fmt::println(buf);
    }
    close(recv_sock);
    close(acpt_sock);
}

void urg_handler(int signo) {
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    fmt::println("Urgent message: {}", buf);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}