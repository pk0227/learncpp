/*
 * ASYNC HTTP SERVER USING STD::ASYNC
 * 
 * Simple HTTP server handling multiple clients concurrently using async operations.
 * 
 * ARCHITECTURE:
 * - Main thread: Accepts incoming connections in loop
 * - Async tasks: Each client handled by separate async operation
 * - Non-blocking: Can accept new connections while serving existing ones
 * 
 * SOCKET PROGRAMMING BASICS:
 * - socket(): Create endpoint for communication
 * - bind(): Associate socket with address/port
 * - listen(): Mark socket as passive (accepting connections)
 * - accept(): Blocks until client connects, returns new socket
 * 
 * CONCURRENT REQUEST HANDLING:
 * - std::async: Launch handler for each client connection
 * - Futures not stored: Fire-and-forget pattern (potential issue!)
 * - Each client handled independently and concurrently
 * 
 * IMPORTANT ISSUES IN THIS CODE:
 * - Future destructors block! Should store futures or use detached threads
 * - No graceful shutdown mechanism
 * - No error recovery for failed client handlers
 * - Production code would use proper async I/O (epoll, io_uring)
 * 
 * BETTER ALTERNATIVES:
 * - Thread pool instead of unlimited async tasks
 * - Event-driven I/O (select, epoll, kqueue)
 * - Modern async frameworks (Boost.Asio, libuv)
 * 
 * INTERVIEW TOPICS:
 * - Scalability limits of thread-per-connection model
 * - Difference between async and non-blocking I/O
 * - How to implement connection limits
 * - Why async might not be best choice for high-performance servers
 */

#include <arpa/inet.h>
#include <cstring>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

constexpr int PORT = 3000;

class HTTPServer {
public:
  HTTPServer(int port) : port_(port) {}

  void start() {
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1) {
      std::cerr << "Failed to create socket" << std::endl;
      return;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket_, reinterpret_cast<sockaddr *>(&server_address),
             sizeof(server_address)) == -1) {
      std::cerr << "Failed to bind socket" << std::endl;
      return;
    }

    if (listen(server_socket_, SOMAXCONN) == -1) {
      std::cerr << "Failed to listen on socket" << std::endl;
      return;
    }

    std::cout << "Server listening on port " << port_ << std::endl;

    while (true) {
      sockaddr_in client_address;
      socklen_t client_address_size = sizeof(client_address);
      int client_socket =
          accept(server_socket_, reinterpret_cast<sockaddr *>(&client_address),
                 &client_address_size);

      if (client_socket == -1) {
        std::cerr << "Failed to accept connection" << std::endl;
        continue;
      }

      std::future<void> client_future = std::async(
          std::launch::async, &HTTPServer::handle_client, this, client_socket);
    }

    close(server_socket_);
  }

private:
  void handle_client(int client_socket) {
    const char *http_response = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: text/plain\r\n"
                                "Content-Length: 12\r\n"
                                "\r\n"
                                "Hello, World!";

    send(client_socket, http_response, strlen(http_response), 0);
    close(client_socket);
  }

  int port_;
  int server_socket_;
};

int main() {
  HTTPServer server(PORT);
  server.start();
  return 0;
}
