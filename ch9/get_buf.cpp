#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>

#include <unistd.h>
#include <sys/socket.h>

#include <fmt/format.h>

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    int sock;
    int snd_buf, rcv_buf, state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state == -1) {
        error_handling("getsockopt() error");
    }
    
    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state == -1) {
        error_handling("getsockopt() error");
    }
    
    fmt::println("Input buffer size: {}", rcv_buf);
    fmt::println("Output buffer size: {}", snd_buf);
}