#include <cstdio>
#include <iostream>

#include <unistd.h>
#include <signal.h>

#include <fmt/format.h>

void timeout(int sig) {
    if (sig == SIGALRM) {
        fmt::println("Time out!");
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
        fmt::println("wait...");
        sleep(100);
    }
}