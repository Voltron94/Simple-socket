#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>



int main()
{
  std::cout << "Loading..." << std::endl;
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM defines a TCP socket. AF_INET specifies the IPv4 protocol family.

  std::cout << "Network configuration..." << std::endl;
  sockaddr_in serverAddress; // Data structure to store socket address.
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(30500); // htons converts from host byte order to network byte order to make the port understandable by the network.
  serverAddress.sin_addr.s_addr = INADDR_ANY; // Bind to any available local address.

  std::cout << "Binding socket..." << std::endl;

  if (bind(serverSocket, (struct sockaddr*)&serverAddress /*cast: struct sockaddr_in to const struct sockaddr*/, sizeof(serverAddress)) < 0) // bind associates the socket with an IP address + port.
  {
    perror("Bind error");
    close(serverSocket);
    return 1;
  }

  std::cout << "Listening..." << std::endl;

  if (listen(serverSocket, 5) < 0) { // The number specifies the backlog of pending connections before refusal, here 5.
      perror("Listen error");
      close(serverSocket);
      return 1;
  }

  while (true)
  {
    std::cout << "Waiting for a new connection on port 30500..." << std::endl;
    int clientSocket = accept(serverSocket, nullptr, nullptr); // accept() is used to accept a connection request on the listening socket.

    // Accept client connection and display IP address
    if (clientSocket < 0) {
      perror("Accept error");
      break;
    } else {
      std::cout << "Connection established" << std::endl;
    }

    // Communication loop
    while (true)
    {
      char buffer[1024] = { 0 };

      std::cout << "Waiting for message" << std::endl;
      int pckg = recv(clientSocket, buffer, sizeof(buffer), 0); // receive data

      if (pckg <= 0)
      {
        perror("Receive error");
      }
      else
      {
        std::cout << "Message received: " << buffer << std::endl;
      }
    }

    close(clientSocket); // Close client socket
    std::cout << "Connection closed" << std::endl;
  }

  close(serverSocket); // Close server socket in case of error or exit

  return 0;
}
