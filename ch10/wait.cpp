#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>
#include <sys/wait.h>

#include <fmt/format.h>

using namespace std::literals;

int main(int argc, char* argv[]) {
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        return 3;
    } else {
        fmt::println("Child PID: {}", pid);
        pid = fork();
        if (pid == 0) {
            exit(7);
        } else {
            fmt::println("Child PID: {}", pid);
            wait(&status);
            if (WIFEXITED(status)) {
                fmt::println("Child send one: {}", WEXITSTATUS(status));
            }
            wait(&status);
            if (WIFEXITED(status)) {
                fmt::println("Child send two: {}", WEXITSTATUS(status));
            }
            std::this_thread::sleep_for(30s);
        }
    }
}