/**
 * @file epoll_reactor.cpp
 * @brief C10k Capable Epoll Echo Server (Level Triggered)
 *
 * SENIOR CONCEPTS:
 * - O_NONBLOCK Config.
 * - epoll_create1, epoll_ctl, epoll_wait.
 * - Edge Cases: Handling 'Client Disconnect' cleanly.
 * - Architecture: Event Loop (Reactor Pattern).
 */

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define MAX_EVENTS 1024
#define PORT 9090

// Helper: Set FD to Non-Blocking
void set_nonblocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl F_GETFL");
    return;
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    perror("fcntl F_SETFL");
  }
}

int main() {
  // 1. Setup Listening Socket
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0) {
    perror("socket");
    return 1;
  }

  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    return 1;
  }

  set_nonblocking(listen_fd); // Listen FD must be non-blocking too!
  listen(listen_fd, SOMAXCONN);

  // 2. Setup Epoll
  int epoll_fd = epoll_create1(0);
  if (epoll_fd < 0) {
    perror("epoll_create1");
    return 1;
  }

  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLIN; // Level Triggered by default
  ev.data.fd = listen_fd;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
    perror("epoll_ctl: listen_fd");
    return 1;
  }

  std::cout << "[Reactor] Listening on port " << PORT << " (Epoll Mode)...\n";

  // 3. Event Loop
  while (true) {
    // Wait for events (Block indefinitely)
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    if (nfds < 0) {
      perror("epoll_wait");
      break;
    }

    for (int i = 0; i < nfds; ++i) {
      int current_fd = events[i].data.fd;

      if (current_fd == listen_fd) {
        // A. Accept New Connection
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int conn_fd =
            accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);

        if (conn_fd < 0) {
          if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Spurious wakeup, ignore
          } else {
            perror("accept");
          }
          continue;
        }

        set_nonblocking(conn_fd); // CRITICAL for Reactor

        // Register Interest
        ev.events = EPOLLIN; // Monitor Read Readiness
        ev.data.fd = conn_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) < 0) {
          perror("epoll_ctl: conn_fd");
          close(conn_fd);
        }

        // std::cout << "New Client: " << conn_fd << "\n"; // Log spam check
      } else {
        // B. Handle Data
        char buf[1024];
        ssize_t n = read(current_fd, buf, sizeof(buf));

        if (n <= 0) {
          // 0 = EOF (Closed), -1 = Error
          if (n < 0 && errno != EAGAIN) {
            perror("read");
          }
          close(current_fd); // Kernel removes from epoll automatically
          // std::cout << "Closed Client: " << current_fd << "\n";
        } else {
          // Echo back
          write(current_fd, buf, n);
        }
      }
    }
  }

  close(listen_fd);
  close(epoll_fd);
  return 0;
}
