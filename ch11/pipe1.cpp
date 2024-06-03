#include <cstdio>
#include <iostream>

#include <unistd.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 30;

int main(int argc, char* argv[]) {
    int fds[2];
    char str[] = "Who are you?";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds);
    pid = fork();
    if (pid == 0) {
        write(fds[1], str, sizeof(str));
    } else {
        read(fds[0], buf, BUF_SIZE);
        fmt::println(buf);
    }
}