#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUF_SIZE 30

void error_handling(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen("./ch7/file_server.cpp", "rb");
    int serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sd, reinterpret_cast<sockaddr*>(&serv_adr), sizeof(serv_adr));
    listen(serv_sd, 5);

    sockaddr_in6 clnt_adr;
    socklen_t clnt_adr_sz = sizeof(clnt_adr);
    int clnt_sd = accept(serv_sd, reinterpret_cast<sockaddr*>(&clnt_adr), &clnt_adr_sz);

    int read_cnt;
    char buf[BUF_SIZE];
    while (true) {
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            write(clnt_sd, buf, read_cnt);
            break;
        }
        write(clnt_sd, buf, BUF_SIZE);
    }

    shutdown(clnt_sd, SHUT_WR);
    read(clnt_sd, buf, BUF_SIZE);
    printf("Message from client: %s\n", buf);

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
}