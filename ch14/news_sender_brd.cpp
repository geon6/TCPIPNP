#include <cstdlib>
#include <string_view>
#include <string>
#include <fstream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <fmt/format.h>

void error_handling(std::string_view msg);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fmt::println("Usage: {} <Broadcast IP> <PORT>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in broad_addr{
        .sin_family{AF_INET},
        .sin_port{htons(atoi(argv[2]))},
        .sin_addr{in_addr{inet_addr(argv[1])}}
    };

    int so_brd = 1;
    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));

    std::ifstream ifs("./ch14/news.txt");
    if (!ifs) {
        error_handling("std::ifstream error");
    }

    std::string str;
    while (std::getline(ifs, str)) {
        sendto(send_sock, str.data(), str.size(), 0, (struct sockaddr*)&broad_addr, sizeof(broad_addr));
        sleep(2);
    }

    ifs.close();
    close(send_sock);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
