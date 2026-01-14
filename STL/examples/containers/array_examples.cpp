/**
 * std::array Examples - Fixed-Size Array
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 array_examples.cpp -o
 * array_examples Run: ./array_examples
 */

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>

int main() {
  std::cout << "=== std::array Examples ===\n\n";

  // 1. Declaration and Initialization
  {
    std::cout << "1. Declaration and Initialization\n";
    std::array<int, 5> arr1 = {1, 2, 3, 4, 5};
    std::array<int, 5> arr2{10, 20, 30, 40, 50};
    std::array<int, 5> arr3{}; // All zeros

    std::cout << "   arr1: ";
    for (int x : arr1)
      std::cout << x << " ";
    std::cout << "\n   arr3 (default): ";
    for (int x : arr3)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 2. Access Elements
  {
    std::cout << "2. Access Elements\n";
    std::array<int, 5> arr = {1, 2, 3, 4, 5};

    std::cout << "   arr[0] = " << arr[0] << " (no bounds check)\n";
    std::cout << "   arr.at(0) = " << arr.at(0) << " (bounds check)\n";
    std::cout << "   arr.front() = " << arr.front() << "\n";
    std::cout << "   arr.back() = " << arr.back() << "\n";
    std::cout << "   arr.data() = " << arr.data() << " (pointer to data)\n\n";
  }

  // 3. Size and Capacity
  {
    std::cout << "3. Size and Capacity\n";
    std::array<int, 5> arr = {1, 2, 3, 4, 5};

    std::cout << "   size() = " << arr.size() << "\n";
    std::cout << "   max_size() = " << arr.max_size() << "\n";
    std::cout << "   empty() = " << (arr.empty() ? "true" : "false") << "\n\n";
  }

  // 4. Fill
  {
    std::cout << "4. Fill\n";
    std::array<int, 5> arr;
    arr.fill(42);

    std::cout << "   After fill(42): ";
    for (int x : arr)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 5. Swap
  {
    std::cout << "5. Swap\n";
    std::array<int, 3> arr1 = {1, 2, 3};
    std::array<int, 3> arr2 = {10, 20, 30};

    std::cout << "   Before swap:\n";
    std::cout << "     arr1: ";
    for (int x : arr1)
      std::cout << x << " ";
    std::cout << "\n     arr2: ";
    for (int x : arr2)
      std::cout << x << " ";

    arr1.swap(arr2);

    std::cout << "\n   After swap:\n";
    std::cout << "     arr1: ";
    for (int x : arr1)
      std::cout << x << " ";
    std::cout << "\n     arr2: ";
    for (int x : arr2)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 6. Algorithms with array
  {
    std::cout << "6. Algorithms with array\n";
    std::array<int, 5> arr = {5, 2, 8, 1, 9};

    std::sort(arr.begin(), arr.end());
    std::cout << "   Sorted: ";
    for (int x : arr)
      std::cout << x << " ";

    int sum = std::accumulate(arr.begin(), arr.end(), 0);
    std::cout << "\n   Sum: " << sum << "\n\n";
  }

  // 7. Comparison
  {
    std::cout << "7. Comparison\n";
    std::array<int, 3> arr1 = {1, 2, 3};
    std::array<int, 3> arr2 = {1, 2, 3};
    std::array<int, 3> arr3 = {1, 2, 4};

    std::cout << "   arr1 == arr2: " << (arr1 == arr2 ? "true" : "false")
              << "\n";
    std::cout << "   arr1 < arr3: " << (arr1 < arr3 ? "true" : "false")
              << "\n\n";
  }

  // 8. Structured Binding (C++17)
  {
    std::cout << "8. Structured Binding (C++17)\n";
    std::array<int, 3> point = {10, 20, 30};
    auto [x, y, z] = point;

    std::cout << "   x=" << x << ", y=" << y << ", z=" << z << "\n\n";
  }

  // 9. Interview Use Case: Fixed-size buffer
  {
    std::cout << "9. Interview Use Case: Fixed-size buffer\n";
    std::array<char, 256> buffer{};

    const char *message = "Hello, World!";
    std::copy(message, message + 13, buffer.begin());

    std::cout << "   Buffer: " << buffer.data() << "\n\n";
  }

  // 10. Interview Use Case: 3D Point
  {
    std::cout << "10. Interview Use Case: 3D Point\n";
    using Point3D = std::array<double, 3>;

    Point3D p1 = {1.0, 2.0, 3.0};
    Point3D p2 = {4.0, 5.0, 6.0};

    // Distance calculation
    double dist_sq = 0.0;
    for (size_t i = 0; i < 3; ++i) {
      double diff = p2[i] - p1[i];
      dist_sq += diff * diff;
    }

    std::cout << "   Distance squared: " << dist_sq << "\n\n";
  }

  std::cout << "Key Points:\n";
  std::cout << "- Fixed size known at compile time\n";
  std::cout << "- Stack allocated (no heap allocation)\n";
  std::cout << "- Zero overhead over C arrays\n";
  std::cout << "- Provides STL container interface\n";
  std::cout << "- Perfect for small, fixed-size collections\n";

  return 0;
}
