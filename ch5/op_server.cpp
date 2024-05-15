#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(const std::string& msg);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        error_handling("socket() error");

    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));
    if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1) 
        error_handling("bind() error");
    
    if (listen(server_socket, 5) == -1)
        error_handling("listen() error");
    
    sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    memset(&client_address, 0, sizeof(client_address));
    int client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_address), &client_address_size);
    if (client_socket == -1) 
        error_handling("accept() error");
    
    char buf[BUF_SIZE];
    int r;

    // read operand count
    int operand_count;
    r = read(client_socket, buf, BUF_SIZE - 1);
    if (r == -1) 
        error_handling("read operand count error");
    buf[r] = 0;
    operand_count = ntohl(atoi(buf));
    std::cout << "read operand count: " << operand_count << std::endl;

    // read operands
    std::vector<int> operands(operand_count, 0);
    for (int i = 0; i < operand_count; i++) {
        r = read(client_socket, buf, BUF_SIZE - 1);
        if (r == -1)
            error_handling("read operand error");
        buf[r] = 0;
        operands[i] = ntohl(atoi(buf));
        std::cout << "read operand: " << operands[i] << std::endl;
    }

    // read operator
    char op;
    r = read(client_socket, &op, 1);
    if (r == -1) 
        error_handling("read operator error");
    std::cout << "read operator: " << op << std::endl;

    // compute 
    int result, net;
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
        write(client_socket, msg.c_str(), msg.size());
        exit(EXIT_FAILURE);
    }
    std::cout << "result: " << result << std::endl;
    net = htonl(result);

    // write
    int w;
    w = write(client_socket, &net, sizeof(net));
    if (w == -1)
        error_handling("write result error");

    // close
    close(client_socket);
    close(server_socket);
}

void error_handling(const std::string& msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}
