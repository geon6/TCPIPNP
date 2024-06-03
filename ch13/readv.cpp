#include <cstdio>
#include <iostream>

#include <sys/uio.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 100;

int main(int argc, char* argv[]) {
    struct iovec vec[2];
    char buf1[BUF_SIZE] = {0, };
    char buf2[BUF_SIZE] = {0, };

    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = BUF_SIZE;

    int str_len = readv(0, vec, 2);

    fmt::println("Read bytes: {}", str_len);
    fmt::println("First message: {}", buf1);
    fmt::println("Second message: {}", buf2);
}