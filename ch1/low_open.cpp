#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>

#include <fcntl.h>
#include <unistd.h>

#include <fmt/format.h>

void error_handling(std::string_view msg);

int main(int argc, char *argv[]) {
    int fd;
    const std::string str{"Let's go!\n"};

    fd = open("./ch1/data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if (fd == -1) {
        error_handling("open() error!");
    }
    fmt::println("file descriptor: {}", fd);

    if (write(fd, str.data(), str.size()) == -1) {
        error_handling("write() error!");
    }
    close(fd);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
