#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(const char *msg);

int main(int argc, char *argv[]) {
    int serv_sock;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_addr;
    int str_len;

    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    
    while (1) {
        fputs("Input message(Q to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);

        if (strcmp(buf, "q\n") == 0 || !strcmp(buf, "Q\n") == 0) 
            break;
        
        write(serv_sock, buf, strlen(buf));
        str_len = read(serv_sock, buf, BUF_SIZE - 1);
        buf[str_len] = 0;
        printf("Message from server: %s", buf);
    }
    close(serv_sock);
}

void error_handling(const char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
}
