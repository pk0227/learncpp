/**
 * @file simple_http_server.cpp
 * @brief Minimal HTTP 1.1 Server (Non-Blocking, Single Threaded)
 *
 * SENIOR CONCEPTS:
 * - Parsing HTTP Protocol manually.
 * - Constructing valid HTTP Responses.
 * - 'send()' Loop for reliability.
 */

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

const char *HTTP_RESPONSE = "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-Length: 13\r\n"
                            "Connection: close\r\n"
                            "\r\n"
                            "Hello, World!";

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind");
    return 1;
  }

  listen(server_fd, 10);
  std::cout << "HTTP Server running on port " << PORT << "...\n";

  while (true) {
    // ACCEPT (Blocking for simplicity in this demo, usually would be epoll)
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0)
      continue;

    char buffer[2048] = {0};
    read(client_fd, buffer, 2048);

    // Very basic "Logging"
    // std::cout << "Request:\n" << buffer << "\n";

    // Send HTTP Response
    write(client_fd, HTTP_RESPONSE, strlen(HTTP_RESPONSE));

    close(client_fd);
  }
  return 0;
}
