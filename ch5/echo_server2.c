#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(const char *msg);

// socket bind listen accept
int main(int argc, char *argv[]) {
    int server_socket, client_socket;
    char buf[BUF_SIZE];
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len;
    int msg_len;

    if (argc != 2) {
        printf("Usage: %s <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        error_handling("socket() error");

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
        error_handling("bind() error");
    
    if (listen(server_socket, 2) == -1)
        error_handling("listen() error");
    
    client_address_len = sizeof(client_address);
    for (int i = 0; i < 4; i++) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket == -1) 
            error_handling("accpet() error");
        else
            printf("Connected client: %d", i + 1);

        while (msg_len = read(client_socket, buf, BUF_SIZE)) {
            if (msg_len == -1)
                error_handling("read() error");
            
            int w = write(client_socket, buf, msg_len);
            if (w == -1) 
                error_handling("write() error");
        }
        close(client_socket);
    }
    close(server_socket);
}

void error_handling(const char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
}
