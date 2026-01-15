/**
 * @file type_erasure.cpp
 * @brief Type Erasure Idiom (Duck Typing in C++)
 *
 * THE PROBLEM:
 * You want to store a collection of UNRELATED objects that share a similar
 * conceptual interface (e.g., "Drawable"), but they do NOT share a common base
 * class.
 *
 * THE SOLUTION:
 * "Type Erasure" pattern (used by std::function, std::any).
 * - A template constructor accepts ANY type T.
 * - An internal abstract base class holds the interface.
 * - An internal template subclass wraps the concrete object T.
 *
 * RESULT:
 * Non-intrusive polymorphism. Classes don't need to inherit from anything!
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ===================================
// Unrelated Concrete Classes
// ===================================
struct Robot {
  void render() const { std::cout << "Robot: BEEP BOOP\n"; }
};

struct Ghost {
  void render() const { std::cout << "Ghost: BOO!\n"; }
};

// ===================================
// The Type-Erased Wrapper "Drawable"
// ===================================
class Drawable {
public:
  // Template Constructor: Accepts anything that has a .render() method
  template <typename T>
  Drawable(T x) : self_(std::make_shared<Model<T>>(std::move(x))) {}

  // Public Interface
  void draw() const { self_->draw_impl(); }

private:
  // The Internal Interface
  struct Concept {
    virtual ~Concept() = default;
    virtual void draw_impl() const = 0;
  };

  // The Bridge Template
  template <typename T> struct Model : Concept {
    T data_;
    Model(T x) : data_(std::move(x)) {}
    void draw_impl() const override {
      // Forward call to the concrete type
      data_.render();
    }
  };

  std::shared_ptr<const Concept> self_;
};

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- Type Erasure Demo ---\n";

  std::vector<Drawable> screen;

  // We can add completely unrelated types!
  screen.emplace_back(Robot{});
  screen.emplace_back(Ghost{});

  // We can iterate and call the common interface
  for (const auto &obj : screen) {
    obj.draw();
  }

  return 0;
}
