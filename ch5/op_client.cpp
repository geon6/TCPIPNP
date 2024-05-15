#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <vector>

#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(const std::string& msg);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) 
        error_handling("connect() error");
    else
        std::cout << "Connected......" << std::endl;

    // input operand count
    int operand_count;
    std::cout << "Operand count: ";
    std::cin >> operand_count;

    // input operands
    std::vector<int> operands(operand_count, 0);
    for (int i = 0; i < operand_count; i++) {
        std::cout << "Operand " << i + 1 << ": ";
        std::cin >> operands[i];
    }

    // input operator
    char op;
    std::cout << "Operator: ";
    std::cin >> op;

    // send to server
    int w, net;
    net = htonl(operand_count);
    w = write(sock, &net, sizeof(net));
    if (w == -1)
        error_handling("write operand count error");

    for (int i = 0; i < operand_count; i++) {
        net = htonl(operands[i]);
        w = write(sock, &net, sizeof(net));
        if (w == -1)
            error_handling("write operand error");
    }
    w = write(sock, &op, sizeof(op));
    if (w == -1)
        error_handling("write operator error");

    // output result
    int r, result;
    char buf[BUF_SIZE];
    r = read(sock, buf, BUF_SIZE);
    if (r == -1)
        error_handling("read result error");
    buf[r] = 0;
    result = atoi(buf);
    std::cout << "Operation result: " << result << std::endl;
}

void error_handling(const std::string& msg) {
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}
