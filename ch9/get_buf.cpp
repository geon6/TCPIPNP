#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/socket.h>

void error_handling(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    int sock;
    int snd_buf, rcv_buf, state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state == -1)
        error_handling("getsockopt() error");
    
    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state == -1)
        error_handling("getsockopt() error");
    
    printf("Input buffer size: %d\n", rcv_buf);
    printf("Output buffer size: %d\n", snd_buf);
}