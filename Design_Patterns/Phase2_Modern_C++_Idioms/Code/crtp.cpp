/**
 * @file crtp.cpp
 * @brief Curiously Recurring Template Pattern (CRTP)
 *
 * THE PROBLEM (Dynamic Polymorphism Overhead):
 * Virtual functions (Runtime polymorphism) have a cost:
 * 1. Extra memory (Search for vptr)
 * 2. Extra indirection (vtable lookup)
 * 3. Prevents inlining (Compiler can't optimize easily)
 *
 * THE SOLUTION (Static Polymorphism):
 * CRTP allows "Virtual-like" behavior resolved at COMPILE TIME.
 *
 * Pattern: Class Derived inherits from Template Base<Derived>.
 *
 * WHEN TO USE:
 * - High-frequency function calls (millions per second).
 * - Image processing, Math libraries, Hardware Abstraction Layers (HAL).
 */

#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

// ===================================
// The CRTP Base Class
// ===================================
template <typename Derived> class ShapeCRTP {
public:
  // The Interface provided to clients
  void draw() const {
    // Cast *this to the Derived type at compile time
    // No vptr, no vtable lookup. Direct function call after inlining.
    static_cast<const Derived *>(this)->draw_impl();
  }
};

// ===================================
// Concrete Implementations
// ===================================
class Circle : public ShapeCRTP<Circle> {
public:
  void draw_impl() const {
    // std::cout << "Drawing Circle\n"; // Commented out for benchmark speed
    volatile int x = 1 + 1; // Dummy work
  }
};

class Square : public ShapeCRTP<Square> {
public:
  void draw_impl() const { volatile int x = 2 + 2; }
};

// ===================================
// Comparison: Traditional Virtuals
// ===================================
class ShapeVirtual {
public:
  virtual ~ShapeVirtual() = default;
  virtual void draw() const = 0;
};

class CircleVirtual : public ShapeVirtual {
public:
  void draw() const override { volatile int x = 1 + 1; }
};

// ===================================
// DEMONSTRATION & BENCHMARK
// ===================================

template <typename T> void run_crtp_benchmark(size_t iterations) {
  T shape;
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    shape.draw();
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "CRTP Time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
                   .count()
            << " us\n";
}

void run_virtual_benchmark(size_t iterations) {
  std::unique_ptr<ShapeVirtual> shape = std::make_unique<CircleVirtual>();
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < iterations; ++i) {
    shape->draw();
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Virtual Time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
                   .count()
            << " us\n";
}

int main() {
  std::cout << "--- CRTP vs Virtual Benchmark ---\n";
  size_t N = 100'000'000;

  // CRTP should generally be faster due to inlining and lack of indirection,
  // though modern CPU branch prediction is very good at optimizing virtual
  // calls too.
  run_crtp_benchmark<Circle>(N);
  run_virtual_benchmark(N);

  std::cout << "\nNote: CRTP enables 'Static Polymorphism' where the exact "
               "type is known at compile time.\n";

  return 0;
}
