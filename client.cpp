#include <cstring>
#include <iostream>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

int main() {
    bool loop = true;
    bool connection = false;

    std::cout << "Network configuration" << std::endl;

    sockaddr_in serverAddress;
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // Create TCP socket
    if (clientSocket < 0) {
        perror("Socket creation error");
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(30500);
    serverAddress.sin_addr.s_addr = inet_addr("192.168.1.86");

    std::cout << "Connecting..." << std::endl;

    int result = -1;
    while (!connection) {
        result = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        if (result == 0) {
            std::cout << "Connection successful" << std::endl;
            connection = true;
        } else {
            perror("Connection error");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    while (loop) {
        std::string message;
        std::cout << "Enter message to send (type 'exit' to quit): ";
        std::getline(std::cin, message);

        int sentBytes = send(clientSocket, message.c_str(), message.size(), 0);
        if (sentBytes < 0) {
            perror("Send error");
            break;
        }

        if (message == "exit") {
            std::cout << "Closing connection." << std::endl;
            close(clientSocket);
            break;
        }
    }

    return 0;
}
