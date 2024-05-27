#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char *message);

int main(int argc, char *argv[]) {

    if (argc == 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) 
        error_handling("socket() error");
    
    sockaddr_in my_adr;
    memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, reinterpret_cast<sockaddr*>(&my_adr), sizeof(my_adr)) == -1) 
        error_handling("bind() error");

    sockaddr_in your_adr;
    char message[BUF_SIZE];
    socklen_t adr_sz;
    int str_len;
    for (int i = 0; i < 3; i++) {
        sleep(5);
        adr_sz = sizeof(your_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, 
            reinterpret_cast<sockaddr*>(&your_adr), &adr_sz);
        printf("Message: %d: %s\n", i + 1, message);
    }
    close(sock);
}

void error_handling(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}
