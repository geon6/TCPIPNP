#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        error_handling("connect() error");
    }

    write(sock, "123", strlen("123"));
    send(sock, "4", strlen("4"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    close(sock);
}