#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <fmt/format.h>

void read_childproc(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        fmt::println("Removed proc id: {}", id);
        fmt::println("Child send: {}", WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]) {
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if (pid == 0) {
        fmt::println("Hi! I'm child process");
        sleep(10);
        return 12;
    } else {
        fmt::println("Child proc id: {}", pid);
        pid = fork();
        if (pid == 0) {
            fmt::println("Hi! I'm child process");
            sleep(10);
            exit(24);
        } else {
            fmt::println("Child proc id: {}", pid);
            for (int i = 0; i < 5; i++) {
                fmt::println("wait...");
                sleep(5);
            }
        }
    }
}