#include <cstdio>
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
        std::this_thread::sleep_for(15s);
        return 24;
    } else {
        while (!waitpid(-1, &status, WNOHANG)) {
            std::this_thread::sleep_for(3s);
            std::cout << "sleep 3sec." << std::endl;
        }
        if (WIFEXITED(status)) {
            std::cout << "Child send: " << WEXITSTATUS(status) << std::endl;
        }
    }
}