/**
 * @file dns_resolver.cpp
 * @brief Modern DNS Resolution using getaddrinfo (IPv4 + IPv6)
 *
 * SENIOR CONCEPTS:
 * - Proper hints setup (AI_ALL, V4MAPPED implied by AF_UNSPEC).
 * - Iterating the linked list of results.
 * - Thread-safe error reporting (gai_strerror).
 */

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

void print_ip(const std::string &hostname) {
  std::cout << "Resolving: " << hostname << "...\n";

  struct addrinfo hints, *res, *p;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;     // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP preferred (though IPs are generic)

  // BLOCKING CALL
  int status = getaddrinfo(hostname.c_str(), NULL, &hints, &res);

  if (status != 0) {
    std::cerr << " [Error] getaddrinfo: " << gai_strerror(status) << "\n";
    return;
  }

  for (p = res; p != NULL; p = p->ai_next) {
    char ipstr[INET6_ADDRSTRLEN];
    void *addr_ptr;
    const char *ipver;

    if (p->ai_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr_ptr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr_ptr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    inet_ntop(p->ai_family, addr_ptr, ipstr, sizeof(ipstr));
    std::cout << " [" << ipver << "] " << ipstr << "\n";
  }

  freeaddrinfo(res);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: ./dns_resolver <hostname>\n";
    std::cerr << "Example: ./dns_resolver google.com\n";
    return 1;
  }

  print_ip(argv[1]);
  return 0;
}
