#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>

#include <fcntl.h>
#include <unistd.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 100;

void error_handling(std::string_view msg);

int main(int argc, char* argv[]) {
    int fd;
    std::string str(BUF_SIZE, '\0');

    fd = open("./ch1/data.txt", O_RDONLY);
    if (fd == -1) {
        error_handling("open() error!");
    }
    fmt::println("file descriptor: {}", fd);

    if (read(fd, str.data(), str.size()) == -1) {
        error_handling("read() error!");
    }
    fmt::print("file data: {}", str);
    close(fd);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
