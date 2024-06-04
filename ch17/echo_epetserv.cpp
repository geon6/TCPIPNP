#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 4;
constexpr int EPOLL_SIZE = 50;

void setnonblockingmode(int fd) {
    int flag = fcntl(fd, F_GETFL, NULL);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

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
    setnonblockingmode(server_socket);
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &event);
    char buf[BUF_SIZE];
    while (true) {
        int event_count = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_count == -1) {
            error_handling("epoll_wait() error");
        }
        fmt::println("epoll_wait() has {} events", event_count);
        for (int i = 0; i < event_count; i++) {
            int fd = ep_events[i].data.fd;
            if (fd == server_socket) {
                sockaddr_in client_address;
                socklen_t client_address_len = sizeof(client_address);
                int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_len);
                setnonblockingmode(client_socket);
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_socket;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &event);
                fmt::println("new connection: {}", client_socket);
            } else {
                while (true) {
                    int len = read(fd, buf, BUF_SIZE);
                    if (len == -1) {
                        if (errno == EAGAIN) {
                            break;
                        }
                    } else if (len == 0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                        close(fd);
                        fmt::println("close connection: {}", fd);
                    } else {
                        write(fd, buf, len);
                    }
                }
            }
        }
    }
    close(server_socket);
    close(epfd);
} 