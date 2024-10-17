# Makefile to compile and run C, C++, Rust, and Go HTTP servers

# Compilers
CC = gcc
CXX = g++
RUSTC = rustc
GO = go

# Source files and output binaries
C_SRC = http_server.c
C_BIN = http_server_c

CPP_SRC = http_server.cpp
CPP_BIN = http_server_cpp

RUST_SRC = http_server.rs
RUST_BIN = http_server_rs

GO_SRC = http_server.go
GO_BIN = http_server_go

# Default target
.PHONY: all
all: build_all

# Build all servers
.PHONY: build_all
build_all: build_c build_cpp build_rust build_go

# Build C server
.PHONY: build_c
build_c:
	@echo "Compiling C server..."
	$(CC) -o $(C_BIN) $(C_SRC)

# Build C++ server
.PHONY: build_cpp
build_cpp:
	@echo "Compiling C++ server..."
	$(CXX) -o $(CPP_BIN) $(CPP_SRC)

# Build Rust server
.PHONY: build_rust
build_rust:
	@echo "Compiling Rust server..."
	$(RUSTC) -o $(RUST_BIN) $(RUST_SRC)

# Build Go server
.PHONY: build_go
build_go:
	@echo "Compiling Go server..."
	$(GO) build -o $(GO_BIN) $(GO_SRC)

# Run all servers
.PHONY: run_all
run_all: run_c run_cpp run_rust run_go

# Run C server
.PHONY: run_c
run_c:
	@echo "Starting C server on port 8080..."
	@./$(C_BIN) &
	@echo $$! > c_server.pid

# Run C++ server
.PHONY: run_cpp
run_cpp:
	@echo "Starting C++ server on port 8081..."
	@./$(CPP_BIN) &
	@echo $$! > cpp_server.pid

# Run Rust server
.PHONY: run_rust
run_rust:
	@echo "Starting Rust server on port 8082..."
	@./$(RUST_BIN) &
	@echo $$! > rust_server.pid

# Run Go server
.PHONY: run_go
run_go:
	@echo "Starting Go server on port 8083..."
	@./$(GO_BIN) &
	@echo $$! > go_server.pid

# Stop all servers
.PHONY: stop_all
stop_all:
	@echo "Stopping all servers..."
	-@kill `cat c_server.pid` 2>/dev/null || true
	-@kill `cat cpp_server.pid` 2>/dev/null || true
	-@kill `cat rust_server.pid` 2>/dev/null || true
	-@kill `cat go_server.pid` 2>/dev/null || true
	@rm -f c_server.pid cpp_server.pid rust_server.pid go_server.pid

# Clean all build artifacts
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(C_BIN) $(CPP_BIN) $(RUST_BIN) $(GO_BIN)
	rm -f c_server.pid cpp_server.pid rust_server.pid go_server.pid

# Phony targets
.PHONY: all build_all build_c build_cpp build_rust build_go run_all run_c run_cpp run_rust run_go stop_all clean
