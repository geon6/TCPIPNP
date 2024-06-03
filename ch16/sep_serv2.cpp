#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

constexpr int BUF_SIZE = 1024;

int main(int argc, char* argv[]) {
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr{
        .sin_family{AF_INET},
        .sin_port{htons(atoi(argv[1]))},
        .sin_addr{in_addr{htonl(INADDR_ANY)}}
    };

    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 5);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_len);

    FILE* readfp = fdopen(clnt_sock, "r");
    FILE* writefp = fdopen(dup(clnt_sock), "w");

    fputs("From server: Hi~ client?\n", writefp);
    fputs("I want to go to ... \n", writefp);
    fflush(writefp);
    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);

    char buf[BUF_SIZE] = {0};
    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);
}