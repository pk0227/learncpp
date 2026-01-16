/**
 * @file unix_socket_ipc.cpp
 * @brief High-Performance Local IPC using UNIX Domain Sockets
 *
 * SENIOR CONCEPTS:
 * - struct sockaddr_un vs sockaddr_in.
 * - unlink() (Cleaning up the socket file).
 * - "Connected" semantics on a file path.
 */

#include <chrono>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h> // for unlink

#define SOCKET_PATH "/tmp/demo_ipc.sock"

void server_process() {
  int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  // CRITICAL: Unlink if exists, or bind fails
  unlink(SOCKET_PATH);

  bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
  listen(listen_fd, 5);

  std::cout << "[Server] Listening on " << SOCKET_PATH << "...\n";

  while (true) {
    int conn_fd = accept(listen_fd, NULL, NULL);
    if (conn_fd > 0) {
      const char *msg = "Hello from Kernel Local IPC!";
      write(conn_fd, msg, strlen(msg));
      close(conn_fd);
    }
  }
}

int main() {
  // Start Server in thread
  std::thread t(server_process);
  t.detach();

  // Give server time to start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Client Logic
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
    char buf[100];
    int n = read(sock_fd, buf, sizeof(buf));
    buf[n] = '\0';
    std::cout << "[Client] Received: " << buf << "\n";
  } else {
    perror("[Client] Connect failed");
  }

  close(sock_fd);

  // Cleanup the file (Good practice)
  unlink(SOCKET_PATH);
  return 0;
}
