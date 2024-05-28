#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>
#include <signal.h>

using namespace std::literals;

void timeout(int sig) {
    if (sig == SIGALRM) {
        std::cout << "Time out!" << std::endl;
    }
    alarm(2);
}

void keycontrol(int sig) {
    if (sig == SIGINT) {
        std::cout << "CTRL + C pressed" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for (int i = 0; i < 3; i++) {
        std::cout << "wait..." << std::endl;
        // std::this_thread::sleep_for(30s);
        sleep(30);
    }
}