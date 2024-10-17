/*
 * Simple HTTP Server in Go
 * Listens on port 8083
 * Returns "Hello, Go!" to any HTTP GET request
 *
 * How to compile:
 *     go build -o http_server_go http_server.go
 *
 * How to run:
 *     ./http_server_go
 *
 * Access the server at:
 *     http://localhost:8083
 *
 * Press Ctrl + C to stop the server.
 */

package main

import (
	"fmt"
	"io"
	"net"
	"os"
	"os/signal"
	"syscall"
)

func main() {
	// Listen on TCP port 8083 on all interfaces
	ln, err := net.Listen("tcp", ":8083")
	if err != nil {
		fmt.Println("Error creating listener:", err)
		return
	}
	fmt.Println("HTTP Server in Go listening on port 8083")

	// Channel to handle OS signals
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)

	// Channel to signal the server to stop
	done := make(chan bool, 1)

	// Goroutine to accept connections
	go func() {
		for {
			conn, err := ln.Accept()
			if err != nil {
				select {
				case <-done:
					// Server is shutting down
					return
				default:
					fmt.Println("Error accepting connection:", err)
				}
				continue
			}
			go handleConnection(conn)
		}
	}()

	// Wait for Ctrl + C (SIGINT)
	<-sigs
	fmt.Println("\nServer terminated gracefully")

	// Signal the server to stop and close the listener
	close(done)
	ln.Close()
}

// Function to handle individual client connections
func handleConnection(conn net.Conn) {
	defer conn.Close()
	buffer := make([]byte, 1024) // Buffer to read client data

	// Read the client's request
	n, err := conn.Read(buffer)
	if err != nil && err != io.EOF {
		fmt.Println("Error reading:", err)
		return
	}
	request := string(buffer[:n])
	fmt.Println("Received request:\n", request)

	// Prepare the response
	response := "HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\nHello, Go!"

	// Send the response to the client
	_, err = conn.Write([]byte(response))
	if err != nil {
		fmt.Println("Error writing response:", err)
		return
	}
	fmt.Println("Sent response:\n", response)
}
