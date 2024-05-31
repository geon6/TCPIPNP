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
    struct sockaddr_in send_addr;
    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.s_addr = inet_addr(argv[1]);
    send_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&send_addr, sizeof(send_addr)) == -1) {
        error_handling("connect() error");
    }

    write(sock, "123", strlen("123"));
    close(sock);
}
