#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 30;

int main(int argc, char* argv[]) {
    std::string str1{"Who are you?"};
    std::string str2{"Thank you for your message"};
    char buf[BUF_SIZE];
    int fds[2];
    pid_t pid;

    pipe(fds);
    pid = fork();
    if (pid == 0) {
        write(fds[1], str1.data(), str1.size() + 1);
        sleep(2);
        read(fds[0], buf, BUF_SIZE);
        fmt::println("Child proc output: ");
    } else {
        read(fds[0], buf, BUF_SIZE);
        fmt::println("Parent proc output: {}", buf);
        write(fds[1], str2.data(), str2.size() + 1);
        sleep(3);
    }
}