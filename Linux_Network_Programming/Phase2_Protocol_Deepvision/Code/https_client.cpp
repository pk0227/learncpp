/**
 * @file https_client.cpp
 * @brief HTTPS Client using OpenSSL (TLS 1.2/1.3)
 *
 * SENIOR CONCEPTS:
 * - OpenSSL initialization and cleanup
 * - TLS handshake (SSL_connect)
 * - Certificate verification
 * - SNI (Server Name Indication)
 * - Reading HTTP response over TLS
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra https_client.cpp -o https_client -lssl
 * -lcrypto RUN: ./https_client <hostname> [path] Example: ./https_client
 * google.com /
 */

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class HTTPSClient {
private:
  SSL_CTX *ctx;
  SSL *ssl;
  int sock_fd;

  void init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
  }

  void cleanup_openssl() { EVP_cleanup(); }

  SSL_CTX *create_context() {
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);

    if (!ctx) {
      ERR_print_errors_fp(stderr);
      throw std::runtime_error("Unable to create SSL context");
    }

    // Load default trusted CA certificates
    if (!SSL_CTX_set_default_verify_paths(ctx)) {
      ERR_print_errors_fp(stderr);
      throw std::runtime_error("Failed to load CA certificates");
    }

    // Enable certificate verification
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, nullptr);

    return ctx;
  }

  int create_socket(const char *hostname, int port) {
    struct hostent *host = gethostbyname(hostname);
    if (!host) {
      std::cerr << "DNS resolution failed for " << hostname << "\n";
      throw std::runtime_error("DNS error");
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      perror("Socket creation failed");
      throw std::runtime_error("Socket error");
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    memcpy(&addr.sin_addr, host->h_addr, host->h_length);

    if (connect(sock, reinterpret_cast<struct sockaddr *>(&addr),
                sizeof(addr)) < 0) {
      perror("Connection failed");
      close(sock);
      throw std::runtime_error("Connection error");
    }

    return sock;
  }

public:
  HTTPSClient() : ctx(nullptr), ssl(nullptr), sock_fd(-1) {
    init_openssl();
    ctx = create_context();
  }

  ~HTTPSClient() {
    if (ssl) {
      SSL_shutdown(ssl);
      SSL_free(ssl);
    }
    if (sock_fd >= 0) {
      close(sock_fd);
    }
    if (ctx) {
      SSL_CTX_free(ctx);
    }
    cleanup_openssl();
  }

  void connect(const char *hostname, int port = 443) {
    // 1. Create TCP connection
    sock_fd = create_socket(hostname, port);
    std::cout << "TCP connection established to " << hostname << ":" << port
              << "\n";

    // 2. Create SSL structure
    ssl = SSL_new(ctx);
    if (!ssl) {
      ERR_print_errors_fp(stderr);
      throw std::runtime_error("SSL_new failed");
    }

    // 3. Set SNI (Server Name Indication) - CRITICAL for virtual hosting
    SSL_set_tlsext_host_name(ssl, hostname);

    // 4. Attach socket to SSL
    SSL_set_fd(ssl, sock_fd);

    // 5. Perform TLS handshake
    std::cout << "Starting TLS handshake...\n";
    if (SSL_connect(ssl) <= 0) {
      ERR_print_errors_fp(stderr);
      throw std::runtime_error("TLS handshake failed");
    }

    // 6. Verify certificate
    long verify_result = SSL_get_verify_result(ssl);
    if (verify_result != X509_V_OK) {
      std::cerr << "Certificate verification failed: "
                << X509_verify_cert_error_string(verify_result) << "\n";
      throw std::runtime_error("Certificate error");
    }

    std::cout << "✓ TLS handshake successful\n";
    std::cout << "✓ Cipher: " << SSL_get_cipher(ssl) << "\n";
    std::cout << "✓ TLS Version: " << SSL_get_version(ssl) << "\n";
  }

  void send_http_request(const std::string &hostname, const std::string &path) {
    std::string request =
        "GET " + path + " HTTP/1.1\r\n" + "Host: " + hostname + "\r\n" +
        "User-Agent: HTTPSClient/1.0\r\n" + "Connection: close\r\n" + "\r\n";

    std::cout << "\n--- Sending Request ---\n" << request << "\n";

    if (SSL_write(ssl, request.c_str(), request.length()) <= 0) {
      ERR_print_errors_fp(stderr);
      throw std::runtime_error("SSL_write failed");
    }
  }

  void read_response() {
    char buffer[4096];
    int bytes;

    std::cout << "--- Response ---\n";

    while ((bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0) {
      buffer[bytes] = '\0';
      std::cout << buffer;
    }

    if (bytes < 0) {
      int ssl_error = SSL_get_error(ssl, bytes);
      if (ssl_error != SSL_ERROR_ZERO_RETURN) {
        std::cerr << "\nSSL_read error: " << ssl_error << "\n";
        ERR_print_errors_fp(stderr);
      }
    }

    std::cout << "\n--- End Response ---\n";
  }
};

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3) {
    std::cerr << "Usage: " << argv[0] << " <hostname> [path]\n";
    std::cerr << "Example: " << argv[0] << " google.com /\n";
    return 1;
  }

  const char *hostname = argv[1];
  std::string path = (argc == 3) ? argv[2] : "/";

  try {
    HTTPSClient client;
    client.connect(hostname);
    client.send_http_request(hostname, path);
    client.read_response();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
