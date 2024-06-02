#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>
#include <signal.h>

#include <fmt/format.h>

using namespace std::literals;

void timeout(int sig) {
    if (sig == SIGALRM) {
        fmt::println("Time out!");
    }
    alarm(2);
}

void keycontrol(int sig) {
    if (sig == SIGINT) {
        fmt::println("CTRL + C pressed");
    }
}

int main(int argc, char* argv[]) {
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for (int i = 0; i < 3; i++) {
        fmt::println("wait...");
        // std::this_thread::sleep_for(30s);
        sleep(30);
    }
}