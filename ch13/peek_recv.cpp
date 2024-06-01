#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    int acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in acpt_addr;
    memset(&acpt_addr, 0, sizeof(acpt_addr));
    acpt_addr.sin_family = AF_INET;
    acpt_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_addr.sin_port = htons(atoi(argv[1]));

    if (bind(acpt_sock, (struct sockaddr*)&acpt_addr, sizeof(acpt_addr)) == -1) {
        error_handling("bind() error");
    }
    if (listen(acpt_sock, 5) == -1) {
        error_handling("listen() error");
    }

    struct sockaddr_in recv_addr;
    socklen_t recv_addr_len = sizeof(recv_addr);
    int recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_addr, &recv_addr_len);

    int str_len;
    char buf[BUF_SIZE];
    while (true) {
        str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
        if (str_len > 0) {
            break;
        }
    }
    buf[str_len] = 0;
    std::cout << "Buffering " << str_len << " bytes: " << buf << std::endl;

    str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
    buf[str_len] = 0;
    std::cout << "Read again: " << buf << std::endl;
    close(acpt_sock);
    close(recv_sock);
}
