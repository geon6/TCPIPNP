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
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen("./ch7/file_server.cpp", "rb");
    int serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_adr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = in_addr{htonl(INADDR_ANY)},
        .sin_zero = {0}
    };

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
    fmt::println("Message from client: {}", buf);

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
}