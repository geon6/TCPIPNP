#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>

#include <fmt/format.h>

using namespace std::literals;

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        fmt::println("Hi, I am a child process");
    } else {
        fmt::println("Child Process ID: {}", pid);
        std::this_thread::sleep_for(30s);
    }

    if (pid == 0) {
        fmt::println("End child process");
    } else {
        fmt::println("End parent process");
    }
}