#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <deque>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>

#define BUF_SIZE 30

void error_handling(const char* msg);
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
    struct sockaddr_in recv_addr;
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(atoi(argv[1]));

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
        std::cout << buf << std::endl;
    }
    close(recv_sock);
    close(acpt_sock);
}

void urg_handler(int signo) {
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    std::cout << "Urgent message: " << buf << std::endl;
}

void error_handling(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}