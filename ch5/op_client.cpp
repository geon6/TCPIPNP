#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <vector>

#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fmt/format.h>

constexpr int BUF_SIZE = 1024;

void error_handling(std::string_view msg);

using byte = unsigned char;

template<typename T>
void print_bytes(T t) {
    byte* b = (byte*) &t;    
    for (int i = 0; i < sizeof(T); i++) {
        printf("%.2x", b[i]);
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fmt::println("Usage: {} <ip> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    
    struct sockaddr_in addr{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr = in_addr{inet_addr(argv[1])},
        .sin_zero = {0}
    };

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        error_handling("connect() error");
    } else {
        fmt::println("Connected......");
    }

    // input operand count
    int operand_count;
    fmt::print("Operand count: ");
    std::cin >> operand_count;

    // input operands
    std::vector<int> operands(operand_count, 0);
    for (int i = 0; i < operand_count; i++) {
        fmt::print("Operand {}: ", i + 1);
        std::cin >> operands[i];
    }

    // input operator
    char op;
    fmt::print("Operator: ");
    std::cin >> op;

    // send to server
    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    int w, net, buf_len = 0, offset = sizeof(int);
    net = htonl(operand_count);
    memcpy(&buf[0], &net, sizeof(int));
    for (int i = 0; i < operand_count; i++) {
        net = htonl(operands[i]);
        memcpy(&buf[4 + i * offset], &net, sizeof(int));
    }
    buf[offset * (operand_count + 1)] = op;

    // for (int i = 0; i < offset * (operand_count + 1) + 1; i++) {
    //     print_bytes<char>(buf[i]);
    // }

    w = write(sock, buf, offset * (operand_count + 1) + 1);

    // output result
    int r, result;
    r = read(sock, &result, sizeof(int));
    result = ntohl(result);
    if (r == -1) {
        error_handling("read result error");
    }
    fmt::println("Operation result: {}", result);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
