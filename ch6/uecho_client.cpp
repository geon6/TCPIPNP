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
    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    
    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    char message[BUF_SIZE];
    sockaddr_in from_address;
    socklen_t addr_size;
    int str_len;
    while (1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (strcmp(message, "q\n") == 0 || strcmp(message, "Q\n") == 0)
            break;
        sendto(sock, message, strlen(message), 0, 
            reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
        addr_size = sizeof(from_address);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, 
            reinterpret_cast<sockaddr*>(&from_address), &addr_size);
        message[str_len] = 0;
        printf("Message from server: %s", message);
    }
    close(sock);
}

void error_handling(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}
