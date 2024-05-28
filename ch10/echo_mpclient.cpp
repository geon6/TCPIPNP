#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char* msg);
void read_routine(int sock, char* buf);
void write_routine(int sock, char* buf);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    
    pid_t pid = fork();
    char buf[BUF_SIZE];
    if (pid == 0) {
        write_routine(sock, buf);
    } else {
        read_routine(sock, buf);
    }
    close(sock);
}

void read_routine(int sock, char* buf) {
    while (true) {
        int str_len = read(sock, buf, BUF_SIZE);
        if (str_len == 0)
            return;
        buf[str_len] = 0;
        std::cout << "Message from server: " << buf << std::endl;
    }
}

void write_routine(int sock, char* buf) {
    while (true) {
        std::cin >> buf;
        if (strcmp(buf, "q\n") == 0 || strcmp(buf, "Q\n") == 0) {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
    }
}

void error_handling(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}