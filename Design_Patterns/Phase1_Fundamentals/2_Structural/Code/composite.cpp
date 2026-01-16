/**
 * @file composite.cpp
 * @brief Composite Pattern - File System Hierarchy
 *
 * SENIOR CONCEPTS:
 * - Component interface (uniform treatment)
 * - Leaf (File) vs Composite (Directory)
 * - Recursive operations (getSize, display)
 * - Modern C++ with unique_ptr ownership
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra composite.cpp -o composite
 * RUN: ./composite
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// Component: Base Interface
// ============================================================================
class FileSystemNode {
public:
  virtual ~FileSystemNode() = default;

  virtual std::string getName() const = 0;
  virtual size_t getSize() const = 0;
  virtual void display(int indent = 0) const = 0;

  // Optional: add/remove (only meaningful for Composite)
  // Default: throw exception (safety over transparency)
  virtual void add(std::unique_ptr<FileSystemNode>) {
    throw std::logic_error("Cannot add to a leaf node");
  }
  virtual std::unique_ptr<FileSystemNode> remove(size_t) {
    throw std::logic_error("Cannot remove from a leaf node");
  }
  virtual FileSystemNode *getChild(size_t) const {
    throw std::logic_error("Leaf node has no children");
  }
};

// ============================================================================
// Leaf: File
// ============================================================================
class File : public FileSystemNode {
private:
  std::string name;
  size_t size; // in bytes

public:
  File(std::string n, size_t s) : name(std::move(n)), size(s) {}

  std::string getName() const override { return name; }

  size_t getSize() const override { return size; }

  void display(int indent = 0) const override {
    std::cout << std::string(indent, ' ') << "📄 " << name << " (" << size
              << " bytes)\n";
  }
};

// ============================================================================
// Composite: Directory
// ============================================================================
class Directory : public FileSystemNode {
private:
  std::string name;
  std::vector<std::unique_ptr<FileSystemNode>> children;

public:
  explicit Directory(std::string n) : name(std::move(n)) {}

  std::string getName() const override { return name; }

  // Recursive size calculation
  size_t getSize() const override {
    size_t totalSize = 0;
    for (const auto &child : children) {
      totalSize += child->getSize();
    }
    return totalSize;
  }

  void display(int indent = 0) const override {
    std::cout << std::string(indent, ' ') << "📁 " << name << "/";

    if (children.empty()) {
      std::cout << " (empty)";
    } else {
      std::cout << " (" << getSize() << " bytes total)";
    }
    std::cout << "\n";

    for (const auto &child : children) {
      child->display(indent + 2);
    }
  }

  // Override add/remove to actually work
  void add(std::unique_ptr<FileSystemNode> node) override {
    children.push_back(std::move(node));
  }

  std::unique_ptr<FileSystemNode> remove(size_t index) override {
    if (index >= children.size()) {
      throw std::out_of_range("Index out of range");
    }

    auto node = std::move(children[index]);
    children.erase(children.begin() + index);
    return node;
  }

  FileSystemNode *getChild(size_t index) const override {
    if (index >= children.size()) {
      throw std::out_of_range("Index out of range");
    }
    return children[index].get();
  }

  size_t getChildCount() const { return children.size(); }
};

// ============================================================================
// Main: Demonstrate Composite Pattern
// ============================================================================
int main() {
  std::cout << "=== File System Composite Pattern ===\n\n";

  // Build file system hierarchy
  auto root = std::make_unique<Directory>("root");

  auto home = std::make_unique<Directory>("home");
  home->add(std::make_unique<File>("readme.txt", 1024));
  home->add(std::make_unique<File>("photo.jpg", 2048000));

  auto docs = std::make_unique<Directory>("documents");
  docs->add(std::make_unique<File>("report.pdf", 512000));
  docs->add(std::make_unique<File>("notes.txt", 4096));

  home->add(std::move(docs));

  root->add(std::move(home));
  root->add(std::make_unique<File>("boot.img", 104857600));

  // Display entire tree
  std::cout << "--- File System Structure ---\n";
  root->display();

  // Get total size (recursive)
  std::cout << "\n--- Total Size Calculation ---\n";
  std::cout << "Root total size: " << root->getSize() << " bytes\n";
  std::cout << "(~" << (root->getSize() / 1024.0 / 1024.0) << " MB)\n";

  // Access specific node
  std::cout << "\n--- Accessing Specific Node ---\n";
  auto *homeDir = root->getChild(0);
  std::cout << "First child: " << homeDir->getName() << "\n";
  std::cout << "Size: " << homeDir->getSize() << " bytes\n";

  // Uniform treatment demo
  std::cout << "\n--- Uniform Treatment Demo ---\n";
  std::vector<FileSystemNode *> nodes = {root.get(), root->getChild(0),
                                         root->getChild(1)};

  for (auto *node : nodes) {
    // Same interface for both Files and Directories!
    std::cout << node->getName() << ": " << node->getSize() << " bytes\n";
  }

  // Demonstrate add() safety
  std::cout << "\n--- Type Safety Check ---\n";
  try {
    auto *file = root->getChild(1);                        // boot.img (a File)
    file->add(std::make_unique<File>("illegal.txt", 100)); // Should throw
  } catch (const std::logic_error &e) {
    std::cout << "Caught expected error: " << e.what() << "\n";
  }

  std::cout << "\n=== Demo Complete ===\n";
  return 0;
}
