#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 30;

void error_handling(std::string_view msg);
void read_childproc(int sig);

int main(int argc, char* argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int fds[2];
    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];

    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error");
    }
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }
    
    pipe(fds);
    pid = fork();
    if (pid == 0) {
        FILE* fp = fopen("./ch11/echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        int len;
        for (int i = 0; i < 10; i++) {
            len = read(fds[0], msgbuf, BUF_SIZE);
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    while (1) {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if (clnt_sock == -1) {
            continue;
        } else {
            fmt::println("new client connected...");
        }
        
        pid = fork();
        if (pid == 0) {
            close(serv_sock);
            while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
                write(clnt_sock, buf, str_len);
                write(fds[1], buf, str_len);
            }
            close(clnt_sock);
            fmt::println("client disconnected...");
            return 0;
        } else {
            close(clnt_sock);
        }
    }
    close(serv_sock);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}

void read_childproc(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        fmt::println("Removed proc id: {}", id);
        fmt::println("Child send: {}", WEXITSTATUS(status));
    }
}