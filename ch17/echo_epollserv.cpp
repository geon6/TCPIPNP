#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 100;
constexpr int EPOLL_SIZE = 50;

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    sockaddr_in server_address{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = in_addr{htonl(INADDR_ANY)}
    };

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }
    if (listen(server_socket, 5) == -1) {
        error_handling("listen() error");
    }

    int epfd = epoll_create(EPOLL_SIZE);
    epoll_event* ep_events = new epoll_event[EPOLL_SIZE];
    epoll_event event{
        .events = EPOLLIN,
        .data = {.fd = server_socket}
    };
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &event);
    char buf[BUF_SIZE];
    while (true) {
        int event_count = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_count == -1) {
            error_handling("epoll_wait() error");
        }

        for (int i = 0; i < event_count; i++) {
            int fd = ep_events[i].data.fd;
            if (fd == server_socket) {
                sockaddr_in client_address;
                socklen_t client_address_len = sizeof(client_address);
                int client_socket = accept(fd, (sockaddr*)&client_address, &client_address_len);
                event.events = EPOLLIN;
                event.data.fd = client_socket;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &event);
                fmt::println("new connection socket: {}", client_socket);
            } else {
                int str_len = read(fd, buf, BUF_SIZE);
                if (str_len == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                    fmt::println("close connection: {}", fd);
                } else {
                    write(fd, buf, BUF_SIZE);
                }
            }
        }
    }
    close(server_socket);
    close(epfd);
}