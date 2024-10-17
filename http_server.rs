// Simple HTTP Server in Rust
// Listens on port 8082
// Returns "Hello, Rust!" to any HTTP GET request
//
// How to compile:
//     rustc -o http_server_rs http_server.rs
//
// How to run:
//     ./http_server_rs
//
// Access the server at:
//     http://localhost:8082
//
// Press Ctrl + C to stop the server.

use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};
use std::process::exit;
use std::thread;

// Import required types for FFI
use std::ffi::c_int;

// Declare the external C function for signal handling
extern "C" {
    fn signal(signum: c_int, handler: extern "C" fn(c_int)) -> c_int;
}

// Define signal numbers (platform-specific)
const SIGINT: c_int = 2; // Interrupt signal
const SIGTERM: c_int = 15; // Termination signal

// Signal handler function
extern "C" fn handle_signal(_sig: c_int) {
    // Handle the signal (e.g., Ctrl + C)
    println!("\nServer terminated gracefully");
    exit(0);
}

fn main() {
    // Set up the signal handler for SIGINT and SIGTERM
    unsafe {
        signal(SIGINT, handle_signal);
        signal(SIGTERM, handle_signal);
    }

    // Bind the listener to the address
    let listener = TcpListener::bind("0.0.0.0:8082").expect("Could not bind");
    println!("HTTP Server in Rust listening on port 8082");

    // Accept connections and handle them
    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                // Handle the client in a new thread
                thread::spawn(|| {
                    handle_client(stream).unwrap_or_else(|error| eprintln!("{:?}", error));
                });
            }
            Err(e) => {
                eprintln!("Connection failed: {}", e);
            }
        }
    }

    // Close the listener (unreachable code in this example)
    // Drop the listener explicitly if needed
    drop(listener);
}

// Function to handle client connections
fn handle_client(mut stream: TcpStream) -> std::io::Result<()> {
    let mut buffer = [0; 512]; // Buffer to read client data

    // Read the client's request
    stream.read(&mut buffer)?;
    println!("Received request:\n{}", String::from_utf8_lossy(&buffer));

    // Prepare the response
    let response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, Rust!";

    // Send the response to the client
    stream.write_all(response.as_bytes())?;
    stream.flush()?;
    println!("Sent response:\n{}", response);

    Ok(())
}
