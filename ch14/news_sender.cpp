#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

const int TTL = 64;
const int BUF_SIZE = 30;

void error_handling(const char* msg);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fmt::println("Usage: {} <GroupIP> <PORT>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in mul_adr;
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));

    int time_live = TTL;
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
    FILE* fp;
    if ((fp = fopen("./ch14/news.txt", "r")) == NULL) {
        error_handling("fopen() error");
    }

    char buf[BUF_SIZE];
    while (!feof(fp)) {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
        sleep(2);
    }

    fclose(fp);
    close(send_sock);
}

void error_handling(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}