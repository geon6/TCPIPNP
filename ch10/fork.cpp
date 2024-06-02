#include <cstdio>

#include <unistd.h>

#include <fmt/format.h>

int gval = 10;
int main(int argc, char* argv[]) {
    pid_t pid;
    int lval = 20;
    gval++, lval += 5;

    pid = fork();
    if (pid == 0) { // child
        gval += 2, lval += 2;
    } else {
        gval -= 2, lval -= 2;
    }

    if (pid == 0) {
        fmt::println("Child Proc: [{}, {}]", gval, lval);
    } else {
        fmt::println("Parent Proc: [{}, {}]", gval, lval);
    }
}