#include <cstdio>
#include <chrono>
#include <functional>

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
        FILE* fp1;
        FILE* fp2;
        char buf[BUF_SIZE];
        fp1 = fopen("./ch15/eve.obj", "r");
        fp2 = fopen("./ch15/cpy.txt", "w");

        while (fgets(buf, BUF_SIZE, fp1) != NULL) {
            fputs(buf, fp2);
        }

        fclose(fp1);
        fclose(fp2);
    });
}