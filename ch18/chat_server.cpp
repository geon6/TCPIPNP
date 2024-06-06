#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 100;
constexpr int MAX_CLNT = 256;

void* handle_clnt(void* arg);
void send_msg(const char* msg, int len);
void error_handling(const char* msg);
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&mutex, NULL);
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = in_addr{htonl(INADDR_ANY)}
    };
    if (bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    while (true) {
        sockaddr_in clnt_addr;
        socklen_t clnt_addr_sz = sizeof(clnt_addr);
        int clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_sz);

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_t tid;
        pthread_create(&tid, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(tid);
        fmt::println("Connected client IP: {}", inet_ntoa(clnt_addr.sin_addr));
    }
    close(serv_sock);
}

void* handle_clnt(void* arg) {
    int clnt_sock = *((int*)arg);
    int str_len = 0;
    char msg[BUF_SIZE];
    while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) {
        send_msg(msg, str_len);
    }
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clnt_cnt; i++) { // remove disconnected client
        if (clnt_sock == clnt_socks[i]) {
            while (i++ < clnt_cnt - 1) {
                clnt_socks[i] = clnt_socks[i + 1]; // ?
            }
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);
    return NULL;
}

void send_msg(const char* msg, int len) { // send to all
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clnt_cnt; i++) {
        write(clnt_socks[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}

void error_handling(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}