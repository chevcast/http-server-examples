/*
 * Simple HTTP Server in C++
 * Listens on port 8081
 * Returns "Hello, C++!" to any HTTP GET request
 *
 * How to compile:
 *     g++ -o http_server_cpp http_server.cpp
 *
 * How to run:
 *     ./http_server_cpp
 *
 * Access the server at:
 *     http://localhost:8081
 *
 * Press Ctrl + C to stop the server.
 */

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // For close()

int server_fd; // File descriptor for the server socket

void handle_sigint(int sig) {
  // Close the server socket and exit
  close(server_fd);
  std::cout << "\nServer terminated gracefully" << std::endl;
  exit(0);
}

int main() {
  int new_socket; // File descriptor for the accepted socket
  struct sockaddr_in address;
  int opt = 1;
  socklen_t addrlen = sizeof(address);
  char buffer[1024] = {0}; // Buffer to read client data
  std::string response =
      "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nHello, C++!";

  // Set up the signal handler for Ctrl + C (SIGINT)
  signal(SIGINT, handle_sigint);

  // Create a socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  // Attach socket to port 8081 and allow reuse
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("Setsockopt failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Define the server address
  address.sin_family = AF_INET;         // IPv4
  address.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
  address.sin_port = htons(8081);       // Port 8081

  // Bind the socket to the network address and port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Start listening for client connections
  if (listen(server_fd, 3) < 0) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  std::cout << "HTTP Server in C++ listening on port 8081" << std::endl;

  while (true) {
    // Accept an incoming connection
    if ((new_socket =
             accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
      perror("Accept failed");
      continue;
    }

    // Read the client's request
    read(new_socket, buffer, sizeof(buffer));
    std::cout << "Received request:\n" << buffer << std::endl;

    // Send the response to the client
    send(new_socket, response.c_str(), response.length(), 0);
    std::cout << "Sent response:\n" << response << std::endl;

    // Close the client socket
    close(new_socket);
  }

  // Close the server socket (unreachable code in this example)
  close(server_fd);

  return 0;
}
