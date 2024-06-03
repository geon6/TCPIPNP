#include <cstdio>
#include <chrono>
#include <functional>

#include <fcntl.h>
#include <unistd.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 512;

void print_time(std::function<void()> func) {
    auto t0 = std::chrono::system_clock::now();
    func();
    auto t1 = std::chrono::system_clock::now();
    auto dt = t1 - t0;
    auto dt_ms = std::chrono::duration_cast<std::chrono::milliseconds>(dt);
    fmt::println("cost time: {}ms", dt_ms.count());
}

int main(int argc, char* argv[]) {
    print_time([&]() {
        int fd1, fd2;
        int len;
        char buf[BUF_SIZE];

        fd1 = open("./ch15/eve.obj", O_RDONLY);
        fd2 = open("./ch15/cpy.txt", O_WRONLY | O_CREAT | O_TRUNC);

        while ((len = read(fd1, buf, sizeof(buf))) > 0) {
            write(fd2, buf, len);
        }

        close(fd1);
        close(fd2);
    });
}