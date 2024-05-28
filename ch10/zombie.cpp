#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>

using namespace std::literals;

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        std::cout << "Hi, I am a child process" << std::endl;
    } else {
        std::cout << "Child Process ID: " << pid << std::endl;
        std::this_thread::sleep_for(30s);
    }

    if (pid == 0) {
        std::cout << "End child process" << std::endl;
    } else {
        std::cout << "End parent process" << std::endl;
    }
}