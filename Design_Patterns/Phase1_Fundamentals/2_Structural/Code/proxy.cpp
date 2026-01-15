/**
 * @file proxy.cpp
 * @brief Proxy Pattern (Virtual Proxy / Lazy Loading)
 *
 * SCENARIO:
 * Loading a high-resolution image from disk is expensive.
 * We want to defer loading until the user actually asks to display it.
 */

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

// ===================================
// 1. The Interface
// ===================================
class Image {
public:
  virtual ~Image() = default;
  virtual void display() = 0;
};

// ===================================
// 2. Real Subject (Heavy)
// ===================================
class RealImage : public Image {
  std::string filename_;

  void loadFromDisk() {
    std::cout << "   [HDD] Loading " << filename_ << " (Heavy Operation)...\n";
    std::this_thread::sleep_for(
        std::chrono::milliseconds(1000)); // Simulate delay
  }

public:
  RealImage(const std::string &filename) : filename_(filename) {
    // In this standard implementation, we might load here,
    // OR the Proxy prevents this constructor from running until needed.
    // Let's assume this constructor does the heavy lifting:
    loadFromDisk();
  }

  void display() override {
    std::cout << "   [GPU] Displaying " << filename_ << "\n";
  }
};

// ===================================
// 3. Proxy (Lazy)
// ===================================
class ProxyImage : public Image {
  std::string filename_;
  std::unique_ptr<RealImage> realImage_; // Encapsulates the real object

public:
  ProxyImage(const std::string &filename) : filename_(filename) {}

  void display() override {
    if (!realImage_) {
      std::cout << "[Proxy] Image not loaded yet. Initializing RealImage...\n";
      realImage_ =
          std::make_unique<RealImage>(filename_); // Heavy init happens HERE
    } else {
      std::cout << "[Proxy] Image already cached.\n";
    }
    realImage_->display();
  }
};

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- Proxy Pattern Demo (Lazy Loading) ---\n";

  // 1. Creation is cheap (just stores filename string)
  std::unique_ptr<Image> img = std::make_unique<ProxyImage>("hd_photo.jpg");
  std::cout << "Image object created. (No disk I/O yet)\n";

  std::cout << "\n--- User clicks 'View' ---\n";
  // 2. First Access: Heavy Loading triggers
  img->display();

  std::cout << "\n--- User clicks 'View' again ---\n";
  // 3. Second Access: Fast (Cached)
  img->display();

  return 0;
}
