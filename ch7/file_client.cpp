#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 30;

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fmt::println("Usage: {} <ip> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE* fp = fopen("./ch7/receive.txt", "wb");
    int sd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_adr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = in_addr{inet_addr(argv[1])},
        .sin_zero = {0}
    };

    connect(sd, reinterpret_cast<sockaddr*>(&serv_adr), sizeof(serv_adr));

    int read_cnt;
    char buf[BUF_SIZE];
    while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0) {
        fwrite((void*)buf, 1, read_cnt, fp);
    }
    fmt::println("Received file data");
    write(sd, "Thank you", 10);
    fclose(fp);
    close(sd);
}