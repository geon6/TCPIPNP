#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        std::cout << "Removed proc id: " << id << std::endl;
        std::cout << "Child send: " << WEXITSTATUS(status) << std::endl;
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
        std::cout << "Hi! I'm child process" << std::endl;
        sleep(10);
        return 12;
    } else {
        std::cout << "Child proc id: " << pid << std::endl;
        pid = fork();
        if (pid == 0) {
            std::cout << "Hi! I'm child process" << std::endl;
            sleep(10);
            exit(24);
        } else {
            std::cout << "Child proc id: " << pid << std::endl;
            for (int i = 0; i < 5; i++) {
                std::cout << "wait..." << std::endl;
                sleep(5);
            }
        }
    }
}