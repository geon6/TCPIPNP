#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 1024;
constexpr int SMALL_BUF = 100;

void* request_handler(void* arg);
void send_data(FILE* fp, const char* ct, const char* file_name);
const char* content_type(const char* file);
void send_error(FILE* fp);
void error_handling(const char* msg);

int main(int argc, char* argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char buf[BUF_SIZE];
    pthread_t tid;
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }
    fmt::println("create socket success");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    fmt::println("bind success");
    if (listen(serv_sock, 20) == -1) {
        error_handling("listen() error");
    }
    fmt::println("listen success");

    while (true) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            error_handling("accpet() error");
        }
        fmt::println("Connection Request: {}:{}", 
            inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
        pthread_create(&tid, NULL, request_handler, &clnt_sock);
        pthread_detach(tid);
    }
    close(serv_sock);
}

void* request_handler(void* arg) {
    int clnt_sock = *((int*)arg);
    fmt::println("thread id: {}", pthread_self());
    fmt::println("client socket: {}", clnt_sock);
    char req_line[SMALL_BUF];
    FILE* clnt_read;
    FILE* clnt_write;

    char method[10];
    char ct[15];
    char file_name[30];

    clnt_read = fdopen(clnt_sock, "r");
    if (clnt_read == NULL) {
        error_handling("fdopen client socket as read error");
    }
    fmt::println("open client read fd success");
    clnt_write = fdopen(dup(clnt_sock), "w");
    if (clnt_write == NULL) {
        error_handling("fdopen client socket as write error");
    }
    fmt::println("open client write fd success");
    fgets(req_line, SMALL_BUF, clnt_read);
    fmt::println("get request line: {}", req_line);
    if (strstr(req_line, "HTTP/") == NULL) {
        fmt::println("request header format error");
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }
    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));
    if (strcmp(method, "GET") != 0) {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }
    fclose(clnt_read);
    send_data(clnt_write, ct, file_name);
    return NULL;
}

void send_data(FILE* fp, const char* ct, const char* file_name) {
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server: Linux Web Server\r\n";
    char cnt_len[] = "Content-length: 2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE* send_file;

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    fmt::println("send file: {}", file_name);
    send_file = fopen(file_name, "r");
    if (send_file == NULL) {
        send_error(fp);
        return ;
    }
    /* header */
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    /* request data */
    while (fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}

const char* content_type(const char* file) {
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));
    if (strcmp(extension, "html") == 0 || strcmp(extension, "htm") == 0) {
        return "text/html";
    } else {
        return "text/plain";
    }
}

void send_error(FILE* fp) {
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server\r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
        "<body><font size=+5><br>发生错误!查看请求文件名和请求方式!"
        "</font></body></html>";
    
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fflush(fp);
}

void error_handling(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
