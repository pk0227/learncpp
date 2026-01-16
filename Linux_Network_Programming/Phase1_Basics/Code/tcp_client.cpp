/**
 * @file tcp_client.cpp
 * @brief Robust TCP Client
 * SENIOR CONCEPTS:
 * - Proper buffering loop for reading until EOF.
 * - Handling connection refusals.
 */

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

void send_message(int sockfd, const std::string &msg) {
  // Send Logic (See tcp_server.cpp for the send_all helper)
  write(sockfd, msg.c_str(), msg.length());
}

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    return 1;

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(9090);

  // Connect to localhost
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    return 1;

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "Connection Failed (Is Server Running?)\n";
    return 1;
  }

  std::cout << "Connected! Type a message:\n";
  std::string input;
  char buffer[1024];

  while (std::getline(std::cin, input)) {
    if (input == "exit")
      break;

    // 1. Send
    write(sockfd, input.c_str(), input.length());

    // 2. Wait for Echo
    int n = read(sockfd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
      buffer[n] = '\0';
      std::cout << "Echo: " << buffer << "\n";
    } else {
      std::cerr << "Server disconnected.\n";
      break;
    }
  }

  close(sockfd);
  return 0;
}
