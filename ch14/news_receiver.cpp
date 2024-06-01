#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

const int BUF_SIZE = 30;

void error_handling(const char* msg);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fmt::println("Usage: {} <GroupIP> <PORT>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in adr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = in_addr{htonl(INADDR_ANY)}
    };
    memset(&adr.sin_zero, 0, sizeof(adr.sin_zero));

    if (bind(recv_sock, (struct sockaddr*)&adr, sizeof(adr)) == -1) {
        error_handling("bind() error");
    }
    struct ip_mreq join_adr{
        .imr_multiaddr = in_addr{inet_addr(argv[1])},
        .imr_interface = in_addr{htonl(INADDR_ANY)}
    };

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));

    char buf[BUF_SIZE];
    while (true) {
        int str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
        if (str_len < 0) {
            break;
        }
        buf[str_len] = 0;
        fmt::println(buf);
    }
    close(recv_sock);
}

void error_handling(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}