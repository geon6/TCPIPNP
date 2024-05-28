#include <cstdio>
#include <iostream>

#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
    if (sig == SIGALRM) {
        std::cout << "Time out!" << std::endl;
    }
    alarm(2);
}

int main(int argc, char* argv[]) {
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);

    alarm(2);

    for (int i = 0; i < 3; i++) {
        std::cout << "wait..." << std::endl;
        sleep(100);
    }
}