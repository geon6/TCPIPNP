#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 100;
constexpr int NAME_SIZE = 20;

void* send_msg(void* arg);
void* recv_msg(void* arg);
void error_handling(const char* msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fmt::println("Usage: {} <ip> <port> <name>", argv[0]);
        exit(EXIT_FAILURE);
    }

    sprintf(name, "[%s]", argv[3]);
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = in_addr{inet_addr(argv[1])}
    };
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    }

    pthread_t send_tid, recv_tid;
    pthread_create(&send_tid, NULL, send_msg, (void*)&sock);
    pthread_create(&recv_tid, NULL, recv_msg, (void*)&sock);
    pthread_join(send_tid, NULL);
    pthread_join(recv_tid, NULL);
    close(sock);
}

void* send_msg(void* arg) {
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    while (1) {
        fgets(msg, BUF_SIZE, stdin);
        if (strcmp(msg, "q\n") == 0 || strcmp(msg, "Q\n") == 0) {
            close(sock);
            exit(EXIT_SUCCESS);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void* recv_msg(void* arg) {
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    int str_len;
    while (1){
        str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
        if (str_len == -1) {
            return (void*)-1;
        }
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}

void error_handling(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}