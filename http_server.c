/*
 * Simple HTTP Server in C
 * Listens on port 8080
 * Returns "Hello, C!" to any HTTP GET request
 *
 * How to compile:
 *     gcc -o http_server_c http_server.c
 *
 * How to run:
 *     ./http_server_c
 *
 * Access the server at:
 *     http://localhost:8080
 *
 * Press Ctrl + C to stop the server.
 */

#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // For close()

int server_fd; // File descriptor for the server socket

void handle_sigint(int sig) {
  // Close the server socket and exit
  close(server_fd);
  printf("\nServer terminated gracefully\n");
  exit(0);
}

int main() {
  int new_socket; // File descriptor for the accepted socket
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0}; // Buffer to read client data
  const char *response =
      "HTTP/1.1 200 OK\r\nContent-Length: 9\r\n\r\nHello, C!";

  // Set up the signal handler for Ctrl + C (SIGINT)
  signal(SIGINT, handle_sigint);

  // Create a socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  // Attach socket to port 8080 and allow reuse
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("Setsockopt failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Define the server address
  address.sin_family = AF_INET;         // IPv4
  address.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
  address.sin_port = htons(8080);       // Port 8080

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

  printf("HTTP Server in C listening on port 8080\n");

  while (1) {
    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      perror("Accept failed");
      continue;
    }

    // Read the client's request
    read(new_socket, buffer, sizeof(buffer));
    printf("Received request:\n%s\n", buffer);

    // Send the response to the client
    write(new_socket, response, strlen(response));
    printf("Sent response:\n%s\n", response);

    // Close the client socket
    close(new_socket);
  }

  // Close the server socket (unreachable code in this example)
  close(server_fd);

  return 0;
}
