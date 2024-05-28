#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>
#include <sys/wait.h>

using namespace std::literals;

int main(int argc, char* argv[]) {
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        return 3;
    } else {
        std::cout << "Child PID: " << pid << std::endl;
        pid = fork();
        if (pid == 0) {
            exit(7);
        } else {
            std::cout << "Child PID: " << pid << std::endl;
            wait(&status);
            if (WIFEXITED(status)) {
                std::cout << "Child send one: " << WEXITSTATUS(status) << std::endl;
            }
            wait(&status);
            if (WIFEXITED(status)) {
                std::cout << "Child send two: " << WEXITSTATUS(status) << std::endl;
            }
            std::this_thread::sleep_for(30s);
        }
    }
}