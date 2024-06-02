#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <fmt/format.h>

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fmt::println("Usage: {} <addr>", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    hostent* host = gethostbyaddr(reinterpret_cast<void*>(&addr.sin_addr), 4, AF_INET);
    if (host == nullptr) {
        error_handling("gethostbyaddr error");
    }
    
    fmt::println("Official name: {}", host->h_name);
    for (int i = 0; host->h_aliases[i] != nullptr; i++) {
        fmt::println("Aliases {}: {}", i + 1, host->h_aliases[i]);
    }
    fmt::println("Address type: {}", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET 6");
    for (int i = 0; host->h_addr_list[i] != nullptr; i++) {
        fmt::println("IP addr {}: {}", i + 1, inet_ntoa(*(reinterpret_cast<in_addr*>(host->h_addr_list[i]))));
    }
}