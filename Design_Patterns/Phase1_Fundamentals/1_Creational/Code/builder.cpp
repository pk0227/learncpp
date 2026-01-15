/**
 * @file builder_pattern.cpp
 * @brief Builder Pattern Implementation (Fluent Interface)
 *
 * THE PROBLEM:
 * A class (e.g., HTTPRequest, Pizza, SQLQuery) has many configuration options.
 * Telescoping constructors (constructor with 10 arguments) are ugly and
 * error-prone.
 *
 * THE SOLUTION:
 * - Separate the construction of a complex object from its representation.
 * - Use a "Builder" class with methods like `header()`, `body()`, `timeout()`.
 * - Return `*this` from setters to allow method chaining (Fluent Interface).
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>

// ===================================
// 1. The Complex Product
// ===================================
class HTTPRequest {
public:
  std::string method = "GET";
  std::string url;
  std::string body;
  std::vector<std::pair<std::string, std::string>> headers;
  int timeout_ms = 5000;

  void print() const {
    std::cout << "HTTP Request [" << method << "] " << url << "\n";
    for (const auto &h : headers) {
      std::cout << "  " << h.first << ": " << h.second << "\n";
    }
    if (!body.empty()) {
      std::cout << "  Body: " << body << "\n";
    }
    std::cout << "  Timeout: " << timeout_ms << "ms\n";
  }
};

// ===================================
// 2. The Builder
// ===================================
class HTTPRequestBuilder {
  HTTPRequest
      request_; // Usage of composition: Builder holds the object being built

public:
  HTTPRequestBuilder() {
    // Set reasonable defaults if needed
  }

  HTTPRequestBuilder &method(const std::string &m) {
    request_.method = m;
    return *this; // Enable chaining
  }

  HTTPRequestBuilder &url(const std::string &u) {
    request_.url = u;
    return *this;
  }

  HTTPRequestBuilder &header(const std::string &key, const std::string &value) {
    request_.headers.push_back({key, value});
    return *this;
  }

  HTTPRequestBuilder &body(const std::string &b) {
    request_.body = b;
    return *this;
  }

  HTTPRequestBuilder &timeout(int ms) {
    request_.timeout_ms = ms;
    return *this;
  }

  // Finalize construction
  HTTPRequest build() {
    // Validation logic can go here (e.g., URL cannot be empty)
    if (request_.url.empty()) {
      throw std::runtime_error("URL is required to build HTTPRequest");
    }
    return request_;
  }
};

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- Builder Pattern Demo ---\n";

  try {
    // Fluent Interface in action
    HTTPRequest req = HTTPRequestBuilder()
                          .url("https://api.example.com/data")
                          .method("POST")
                          .header("Content-Type", "application/json")
                          .header("Authorization", "Bearer token123")
                          .body("{ \"id\": 42 }")
                          .timeout(2000)
                          .build();

    req.print();

  } catch (const std::exception &e) {
    std::cerr << "Error building request: " << e.what() << "\n";
  }

  return 0;
}
