/**
 * @file pimpl_pattern.cpp
 * @brief Pointer to Implementation (Pimpl) Idiom
 *
 * THE PROBLEM:
 * 1. Compilation Dependencies: Changing a private member forces recompilation
 * of all users.
 * 2. ABI Stability: Adding members changes class size, breaking binary
 * compatibility.
 * 3. Hiding Details: You want to hide platform-specific headers (e.g.,
 * <windows.h>).
 *
 * THE SOLUTION:
 * - The public class holds a std::unique_ptr<Impl> forward declared.
 * - The Impl class is defined ONLY in the .cpp file.
 *
 * NOTE: This file combines .h and .cpp for demonstration.
 * In production, split `SocketClient` into header and `SocketClient::Impl` into
 * source.
 */

#include <iostream>
#include <memory>
#include <string>

// ===================================
// Header File Part (SocketClient.h)
// ===================================
class SocketClient {
public:
  SocketClient();
  ~SocketClient(); // Important: Must be defined where Impl is complete

  void connect(const std::string &url);
  void send(const std::string &data);

  // Movable but not Copyable (due to unique_ptr)
  SocketClient(SocketClient &&) noexcept;
  SocketClient &operator=(SocketClient &&) noexcept;

  // Copy requires deep copy implementation
  SocketClient(const SocketClient &) = delete;
  SocketClient &operator=(const SocketClient &) = delete;

private:
  class Impl; // Forward declaration
  std::unique_ptr<Impl> pImpl;
};

// ===================================
// Source File Part (SocketClient.cpp)
// ===================================

// The hidden implementation class
class SocketClient::Impl {
public:
  void low_level_connect(const std::string &url) {
    std::cout << "[Internal] Opening OS socket to " << url << "\n";
  }

  void low_level_send(const std::string &data) {
    std::cout << "[Internal] Writing bytes: " << data << "\n";
  }
};

// Constructor: Initialize the hidden pointer
SocketClient::SocketClient() : pImpl(std::make_unique<Impl>()) {}

// Destructor: Must be here where Impl is fully defined
SocketClient::~SocketClient() = default;

// Move operations
SocketClient::SocketClient(SocketClient &&) noexcept = default;
SocketClient &SocketClient::operator=(SocketClient &&) noexcept = default;

// Public interface forwards implementation
void SocketClient::connect(const std::string &url) {
  pImpl->low_level_connect(url);
}

void SocketClient::send(const std::string &data) {
  pImpl->low_level_send(data);
}

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- Pimpl Idiom Demo ---\n";

  SocketClient client;
  client.connect("127.0.0.1:8080");
  client.send("Hello World");

  std::cout << "Client size: " << sizeof(client)
            << " bytes (Size of unique_ptr)\n";

  return 0;
}
