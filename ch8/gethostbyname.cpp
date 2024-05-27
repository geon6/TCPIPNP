#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <addr>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    hostent* host = gethostbyname(argv[1]);
    if (host == nullptr) {
        error_handling("gethostbyname error");
    }
    printf("Official name: %s\n", host->h_name);
    for (int i = 0; host->h_aliases[i] != nullptr; i++) {
        printf("Aliases %d: %s\n", i + 1, host->h_aliases[i]);
    }
    printf("Address type: %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET 6");
    for (int i = 0; host->h_addr_list[i] != nullptr; i++) {
        printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(reinterpret_cast<in_addr*>(host->h_addr_list[i]))));
    }
}