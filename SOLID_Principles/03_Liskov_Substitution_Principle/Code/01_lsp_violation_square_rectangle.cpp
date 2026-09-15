/**
 * @file 01_lsp_violation_square_rectangle.cpp
 * @brief Demonstrates the canonical Liskov Substitution Principle (LSP) violation: Square-Rectangle.
 * 
 * The Mathematical Fallacy in OOP:
 *   - In geometry, a Square "is a" Rectangle with equal sides.
 *   - In Object-Oriented Design, behavioral subtyping governs relationships, NOT mathematical taxonomy.
 *   - A Rectangle has the behavioral contract:
 *       `setWidth(w)` alters width INDEPENDENTLY of height.
 *       `setHeight(h)` alters height INDEPENDENTLY of width.
 *   - When Square derives from Rectangle, maintaining the square invariant (width == height)
 *     forces `setWidth()` to mutate height as a side effect.
 * 
 * The Consequence:
 *   - Any client function accepting `Rectangle&` that assumes independent dimension mutability
 *     breaks catastrophically when passed a `Square&`.
 * 
 * @standard C++20
 */

#include <iostream>
#include <cassert>
#include <memory>

// ============================================================================
// BASE CLASS: Rectangle
// Contract: Width and Height are independent mutable dimensions.
// ============================================================================
class Rectangle {
protected:
    double width_{0.0};
    double height_{0.0};

public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    virtual ~Rectangle() = default;

    virtual void setWidth(double w) { width_ = w; }
    virtual void setHeight(double h) { height_ = h; }

    [[nodiscard]] double getWidth() const noexcept { return width_; }
    [[nodiscard]] double getHeight() const noexcept { return height_; }
    [[nodiscard]] virtual double getArea() const noexcept { return width_ * height_; }
};

// ============================================================================
// ANTI-PATTERN: Square deriving from Rectangle
// ============================================================================
class Square : public Rectangle {
public:
    explicit Square(double side) : Rectangle(side, side) {}

    // Violating Base Postcondition: Side effect mutates height!
    void setWidth(double w) override {
        width_ = w;
        height_ = w; // Forces invariant, but breaks Rectangle contract!
    }

    // Violating Base Postcondition: Side effect mutates width!
    void setHeight(double h) override {
        width_ = h;
        height_ = h; // Forces invariant, but breaks Rectangle contract!
    }
};

// ============================================================================
// CLIENT CODE (Written against Rectangle Contract)
// Invariant assumption: setWidth(5) and setHeight(4) MUST yield area = 20!
// ============================================================================
void transformAndCalculateArea(Rectangle& r) {
    std::cout << "[Client] Setting Width=5.0, Height=4.0...\n";
    r.setWidth(5.0);
    r.setHeight(4.0);

    std::cout << "[Client] Dimensions: " << r.getWidth() << " x " << r.getHeight() << "\n";
    std::cout << "[Client] Expected Area: 20.0 | Actual Area: " << r.getArea() << "\n";

    if (r.getArea() != 20.0) {
        std::cerr << "💥 [CRITICAL ERROR] LSP Violation Detected! Subtype altered supertype behavior!\n";
    } else {
        std::cout << "✅ Rectangle contract satisfied.\n";
    }
}

int main() {
    std::cout << "=== LSP Violation: Square-Rectangle Classic Fallacy ===\n\n";

    std::cout << "--- 1. Testing with Real Rectangle ---\n";
    Rectangle rect(1.0, 1.0);
    transformAndCalculateArea(rect);

    std::cout << "\n--- 2. Substituting Square into Rectangle Reference ---\n";
    Square sq(2.0);
    transformAndCalculateArea(sq); // Breaks the client's invariant!

    return 0;
}
