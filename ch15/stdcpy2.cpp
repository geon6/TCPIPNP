#include <fstream>
#include <string>
#include <chrono>
#include <functional>

#include <fmt/format.h>

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
        std::ifstream ifs("./ch15/eve.obj");
        std::ofstream ofs("./ch15/cpy.txt");
        std::string line;
        while (std::getline(ifs, line)) {
            ofs << line;
        }
        ifs.close();
        ofs.close();
    });
}