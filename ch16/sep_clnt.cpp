#include <cstdio>
#include <cstdlib>

#include <sys/socket.h>
#include <arpa/inet.h>

constexpr int BUF_SIZE = 1024;

int main(int argc, char* argv[]) {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr{
        .sin_family{AF_INET},
        .sin_port{htons(atoi(argv[2]))},
        .sin_addr{in_addr{inet_addr(argv[1])}}
    };

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    FILE* readfp = fdopen(sock, "r");
    FILE* writefp = fdopen(sock, "w");

    char buf[BUF_SIZE];
    while (true) {
        if (fgets(buf, sizeof(buf), readfp) == NULL) {
            break;
        }
        fputs(buf, stdout);
        fflush(stdout);
    }

    fputs("From client: Thank you!\n", writefp);
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);
}