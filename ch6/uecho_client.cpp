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
        printf("Usage: %s <ip> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) 
        error_handling("socket()");

    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    std::string msg;
    socklen_t address_size;
    sockaddr from_address;
    int str_len;
    while (1) {
        std::cout << "Insert message(q to quit): " << std::endl;
        std::cin >> msg;
        if (msg == "q" || msg == "Q")
            break;
        
        sendto(sock, msg.data(), msg.size(), 0, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
        address_size = sizeof(from_address);
        str_len = recvfrom(sock, msg.data(), BUF_SIZE, 0, reinterpret_cast<sockaddr*>(&from_address), &address_size);
        std::cout << "Message from server: " << msg << std::endl;
    }
    close(sock);
}

void error_handling(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}
