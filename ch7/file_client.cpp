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
    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE* fp = fopen("./ch7/receive.txt", "wb");
    int sd = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    connect(sd, reinterpret_cast<sockaddr*>(&serv_adr), sizeof(serv_adr));

    int read_cnt;
    char buf[BUF_SIZE];
    while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0) {
        fwrite((void*)buf, 1, read_cnt, fp);
    }
    puts("Received file data");
    write(sd, "Thank you", 10);
    fclose(fp);
    close(sd);
}