/**
 * Problem 06: Design In-Memory File System - Advanced Data Structures
 *
 * Design an in-memory file system to simulate the following functions:
 * - ls: List directory contents
 * - mkdir: Create directory
 * - addContentToFile: Add content to file
 * - readContentFromFile: Read file content
 *
 * Senior-level focus: Complex nested data structures, path parsing, trie-like
 * structure
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 problem_06_file_system.cpp -o
 * file_system Run: ./file_system
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// ============================================================================
// SOLUTION
// ============================================================================

class FileSystem {
private:
  struct Node {
    bool isFile = false;
    std::string content;
    std::map<std::string, Node *> children; // Ordered for ls()

    ~Node() {
      for (auto &[name, child] : children) {
        delete child;
      }
    }
  };

  Node *root;

  // Parse path into components
  std::vector<std::string> parsePath(const std::string &path) {
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string part;

    while (std::getline(ss, part, '/')) {
      if (!part.empty()) {
        parts.push_back(part);
      }
    }

    return parts;
  }

  // Navigate to node, creating directories if needed
  Node *navigate(const std::string &path, bool createDirs = false) {
    auto parts = parsePath(path);
    Node *curr = root;

    for (const auto &part : parts) {
      if (curr->children.find(part) == curr->children.end()) {
        if (!createDirs)
          return nullptr;
        curr->children[part] = new Node();
      }
      curr = curr->children[part];
    }

    return curr;
  }

public:
  FileSystem() { root = new Node(); }

  ~FileSystem() { delete root; }

  // List directory contents (sorted)
  std::vector<std::string> ls(const std::string &path) {
    Node *node = navigate(path);
    std::vector<std::string> result;

    if (!node)
      return result;

    // If it's a file, return just the filename
    if (node->isFile) {
      auto parts = parsePath(path);
      if (!parts.empty()) {
        result.push_back(parts.back());
      }
      return result;
    }

    // If it's a directory, return sorted children
    for (const auto &[name, child] : node->children) {
      result.push_back(name);
    }

    return result; // Already sorted (map maintains order)
  }

  // Create directory
  void mkdir(const std::string &path) { navigate(path, true); }

  // Add content to file
  void addContentToFile(const std::string &filePath,
                        const std::string &content) {
    auto parts = parsePath(filePath);
    if (parts.empty())
      return;

    // Navigate to parent directory
    std::string dirPath = "/";
    for (size_t i = 0; i < parts.size() - 1; ++i) {
      dirPath += parts[i] + "/";
    }

    Node *parent = navigate(dirPath, true);
    std::string filename = parts.back();

    if (parent->children.find(filename) == parent->children.end()) {
      parent->children[filename] = new Node();
    }

    Node *file = parent->children[filename];
    file->isFile = true;
    file->content += content;
  }

  // Read file content
  std::string readContentFromFile(const std::string &filePath) {
    Node *node = navigate(filePath);
    if (!node || !node->isFile)
      return "";
    return node->content;
  }
};

// ============================================================================
// CONTAINER SELECTION ANALYSIS
// ============================================================================

/*
 * WHY map<string, Node*>?
 *
 * Requirements:
 * - Store directory/file hierarchy
 * - ls() must return sorted results
 * - Fast lookup by name
 * - Maintain order
 *
 * Options:
 *
 * 1. unordered_map<string, Node*>:
 *    - O(1) lookup ✓
 *    - Unordered ✗
 *    - Would need to sort for ls() - O(n log n)
 *    ❌ Extra sorting overhead
 *
 * 2. map<string, Node*>: ✓✓✓
 *    - O(log n) lookup (acceptable)
 *    - Automatically sorted ✓
 *    - ls() is O(n) without sorting ✓
 *    ✅ Perfect for this use case!
 *
 * 3. vector<pair<string, Node*>>:
 *    - O(n) lookup ✗
 *    - Would need sorting
 *    ❌ Too slow
 *
 * KEY INSIGHT: map maintains sorted order, making ls() efficient.
 * The O(log n) lookup is acceptable for file system operations.
 *
 * SENIOR INSIGHT: This is a trie-like structure where each node
 * contains a map of children. Understanding when to use ordered
 * vs unordered containers is crucial.
 */

// ============================================================================
// TEST CASES
// ============================================================================

void testBasicOperations() {
  std::cout << "=== TEST: Basic Operations ===\n";
  FileSystem fs;

  // Create directories
  fs.mkdir("/a/b/c");
  fs.mkdir("/a/b/d");

  // List directory
  auto files = fs.ls("/a/b");
  std::cout << "   ls /a/b: ";
  for (const auto &f : files)
    std::cout << f << " ";
  std::cout << "(should be: c d)\n";
  assert(files.size() == 2);
  assert(files[0] == "c" && files[1] == "d");

  std::cout << "   ✓ Basic operations passed\n\n";
}

void testFileOperations() {
  std::cout << "=== TEST: File Operations ===\n";
  FileSystem fs;

  // Create file and add content
  fs.addContentToFile("/a/b/file.txt", "Hello ");
  fs.addContentToFile("/a/b/file.txt", "World");

  // Read content
  std::string content = fs.readContentFromFile("/a/b/file.txt");
  std::cout << "   Content: \"" << content << "\"\n";
  assert(content == "Hello World");

  // List file
  auto files = fs.ls("/a/b/file.txt");
  std::cout << "   ls /a/b/file.txt: " << files[0] << "\n";
  assert(files.size() == 1);
  assert(files[0] == "file.txt");

  std::cout << "   ✓ File operations passed\n\n";
}

void testComplexHierarchy() {
  std::cout << "=== TEST: Complex Hierarchy ===\n";
  FileSystem fs;

  fs.mkdir("/home/user/documents");
  fs.mkdir("/home/user/downloads");
  fs.addContentToFile("/home/user/documents/readme.txt", "README");
  fs.addContentToFile("/home/user/file.txt", "data");

  auto files = fs.ls("/home/user");
  std::cout << "   ls /home/user: ";
  for (const auto &f : files)
    std::cout << f << " ";
  std::cout << "\n";
  assert(files.size() == 3); // documents, downloads, file.txt

  std::cout << "   ✓ Complex hierarchy passed\n\n";
}

void testSortedOutput() {
  std::cout << "=== TEST: Sorted Output ===\n";
  FileSystem fs;

  // Add in random order
  fs.mkdir("/root/zebra");
  fs.mkdir("/root/apple");
  fs.mkdir("/root/mango");
  fs.mkdir("/root/banana");

  auto files = fs.ls("/root");
  std::cout << "   ls /root (should be sorted): ";
  for (const auto &f : files)
    std::cout << f << " ";
  std::cout << "\n";

  // Verify sorted
  assert(files[0] == "apple");
  assert(files[1] == "banana");
  assert(files[2] == "mango");
  assert(files[3] == "zebra");

  std::cout << "   ✓ Sorted output passed\n\n";
}

// ============================================================================
// COMPLEXITY ANALYSIS
// ============================================================================

void complexityAnalysis() {
  std::cout << "=== COMPLEXITY ANALYSIS ===\n\n";

  std::cout << "Let n = number of path components, m = number of children\n\n";

  std::cout << "ls(path):\n";
  std::cout << "  Time: O(n + m)\n";
  std::cout << "    - Navigate: O(n log m) for each component\n";
  std::cout << "    - List children: O(m) (already sorted)\n";
  std::cout << "  Space: O(m) for result\n\n";

  std::cout << "mkdir(path):\n";
  std::cout << "  Time: O(n log m)\n";
  std::cout << "    - Navigate and create: O(log m) per component\n";
  std::cout << "  Space: O(n) for new nodes\n\n";

  std::cout << "addContentToFile(path, content):\n";
  std::cout << "  Time: O(n log m + k) where k = content length\n";
  std::cout << "  Space: O(k) for content\n\n";

  std::cout << "readContentFromFile(path):\n";
  std::cout << "  Time: O(n log m)\n";
  std::cout << "  Space: O(k) for returned content\n\n";
}

// ============================================================================
// SENIOR-LEVEL INSIGHTS
// ============================================================================

void seniorInsights() {
  std::cout << "=== SENIOR-LEVEL INSIGHTS ===\n\n";

  std::cout << "1. Data Structure Choice:\n";
  std::cout << "   - Trie-like structure with map at each node\n";
  std::cout << "   - map provides automatic sorting for ls()\n";
  std::cout << "   - Trade-off: O(log m) lookup vs O(1) with unordered_map\n\n";

  std::cout << "2. Memory Management:\n";
  std::cout << "   - Manual memory management with new/delete\n";
  std::cout << "   - Could use unique_ptr for automatic cleanup\n";
  std::cout << "   - Destructor recursively deletes tree\n\n";

  std::cout << "3. Path Parsing:\n";
  std::cout << "   - String manipulation with stringstream\n";
  std::cout << "   - Handle edge cases (empty parts, trailing slashes)\n\n";

  std::cout << "4. Design Patterns:\n";
  std::cout << "   - Composite pattern (files and directories)\n";
  std::cout << "   - Recursive tree traversal\n";
  std::cout << "   - Lazy creation (mkdir creates intermediate dirs)\n\n";

  std::cout << "5. Interview Follow-ups:\n";
  std::cout << "   - How to handle concurrent access? (locks, thread-safety)\n";
  std::cout << "   - How to implement file permissions?\n";
  std::cout << "   - How to handle symbolic links?\n";
  std::cout << "   - How to optimize for large directories?\n\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "Design In-Memory File System\n";
  std::cout << "============================\n\n";

  testBasicOperations();
  testFileOperations();
  testComplexHierarchy();
  testSortedOutput();
  complexityAnalysis();
  seniorInsights();

  std::cout << "All tests passed! ✓\n";
  std::cout << "\nThis problem tests:\n";
  std::cout << "- Complex nested data structures\n";
  std::cout << "- map vs unordered_map trade-offs\n";
  std::cout << "- Tree traversal and manipulation\n";
  std::cout << "- String parsing and manipulation\n";
  std::cout << "- Memory management\n";
  std::cout << "- System design thinking\n";

  return 0;
}
