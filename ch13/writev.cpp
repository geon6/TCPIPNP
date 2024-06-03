#include <cstdio>
#include <iostream>

#include <sys/uio.h>

#include <fmt/format.h>

int main(int argc, char* argv[]) {
    struct iovec vec[2];
    char buf1[] = "ABCDEFG";
    char buf2[] = "1234567";
    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;

    int str_len = writev(1, vec, 2);
    fmt::println("Write bytes: {}", str_len);
}