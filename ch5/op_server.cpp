#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

#include <unistd.h>
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
    if (argc != 2) {
        fmt::println("Usage: {} <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in server_address{
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = in_addr{htonl(INADDR_ANY)},
        .sin_zero = {0}
    };
    if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }
    if (listen(server_socket, 5) == -1) {
        error_handling("listen() error");
    }
    
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    int client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_address), &client_address_size);
    if (client_socket == -1) {
        error_handling("accept() error");
    }

    // read data
    int r, net, operand_count, offset = sizeof(int);
    char buf[BUF_SIZE];
    r = read(client_socket, buf, BUF_SIZE - 1);
    // for (int i = 0; i < r; i++) {
    //     print_bytes(buf[i]);
    // }
    if (r == -1) {
        error_handling("read error");
    }
    buf[r] = 0;
    memcpy(&net, &buf[0], sizeof(int));
    operand_count = ntohl(net);
    fmt::println("read operand count: {}", operand_count);
    std::vector<int> operands(operand_count, 0);
    for (int i = 0; i < operand_count; i++) {
        memcpy(&net, &buf[4 + sizeof(int) * i], sizeof(int));
        operands[i] = ntohl(net);
        fmt::println("read operand: {}", operands[i]);
    }
    char op = buf[r - 1];
    fmt::println("read operator: {}", op);

    // compute 
    int result;
    if (op == '+') {
        result = 0;
        for (int i = 0; i < operand_count; i++) {
            result += operands[i];
        }
    } else if (op == '*') {
        result = 1;
        for (int i = 0; i < operand_count; i++) {
            result *= operands[i];
        }
    } else {
        std::string msg = "Unexpected operator\n";
        write(client_socket, msg.data(), msg.size());
        exit(EXIT_FAILURE);
    }
    fmt::println("result: {}", result);
    net = htonl(result);

    // write
    int w;
    w = write(client_socket, &net, sizeof(net));
    if (w == -1) {
        error_handling("write result error");
    }

    // close
    close(client_socket);
    close(server_socket);
}

void error_handling(std::string_view msg) {
    perror(msg.data());
    exit(EXIT_FAILURE);
}
