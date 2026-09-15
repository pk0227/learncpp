/**
 * @file 04_lsp_compile_time_concepts.cpp
 * @brief Demonstrates Compile-Time Liskov Substitution Principle using C++20 Concepts.
 * 
 * In Modern C++, we can enforce Liskov contracts statically rather than discovering
 * contract breaches at runtime:
 *   1. Syntactic Contract: Required method signatures, argument types, and return types.
 *   2. Semantic / Exception Contract: Enforcing `noexcept` guarantees at compile time!
 *   3. Structural Substitutability: Any type satisfying the concept can be substituted
 *      into generic algorithms with zero vtable indirection and 100% inlining.
 * 
 * @standard C++20
 */

#include <iostream>
#include <concepts>
#include <string>
#include <vector>
#include <cmath>

// ============================================================================
// C++20 CONCEPT: Strict Liskov Contract for Geometric Shapes
// Demands:
//   - getArea() must return convertible to double AND MUST BE noexcept!
//   - describe() must return convertible to std::string
// ============================================================================
template <typename T>
concept ShapeContract = requires(const T& shape) {
    { shape.getArea() } noexcept -> std::convertible_to<double>;
    { shape.describe() } -> std::convertible_to<std::string>;
};

// ============================================================================
// COMPLIANT TYPE 1: Circle
// ============================================================================
class Circle {
private:
    double radius_;

public:
    explicit constexpr Circle(double r) noexcept : radius_(r) {}

    [[nodiscard]] double getArea() const noexcept {
        return 3.14159265358979323846 * radius_ * radius_;
    }

    [[nodiscard]] std::string describe() const {
        return "Circle (radius: " + std::to_string(radius_) + ")";
    }
};

// ============================================================================
// COMPLIANT TYPE 2: RightTriangle
// ============================================================================
class RightTriangle {
private:
    double base_;
    double height_;

public:
    constexpr RightTriangle(double b, double h) noexcept : base_(b), height_(h) {}

    [[nodiscard]] double getArea() const noexcept {
        return 0.5 * base_ * height_;
    }

    [[nodiscard]] std::string describe() const {
        return "RightTriangle (" + std::to_string(base_) + " x " + std::to_string(height_) + ")";
    }
};

// ============================================================================
// NON-COMPLIANT TYPE: FlawedShape (Violates noexcept contract!)
// ============================================================================
class FlawedShape {
public:
    // NOT noexcept! May throw exceptions.
    [[nodiscard]] double getArea() const {
        if (std::rand() % 10 == 0) throw std::runtime_error("Calculation fault");
        return 42.0;
    }

    [[nodiscard]] std::string describe() const {
        return "FlawedShape (Throws exceptions)";
    }
};

// ============================================================================
// GENERIC CLIENT ALGORITHM (Guaranteed Liskov Substitutability)
// ============================================================================
template <ShapeContract S>
void processShape(const S& shape) {
    std::cout << "[Generic Pipeline] " << shape.describe() 
              << " | Area: " << shape.getArea() << " (Compiled with zero vtable!)\n";
}

int main() {
    std::cout << "=== Compile-Time LSP Enforcement via C++20 Concepts ===\n\n";

    Circle circle(5.0);
    RightTriangle triangle(4.0, 3.0);

    // Both satisfy ShapeContract and substitute seamlessly
    processShape(circle);
    processShape(triangle);

    // Statically verified concept compliance:
    static_assert(ShapeContract<Circle>, "Circle must satisfy ShapeContract");
    static_assert(ShapeContract<RightTriangle>, "RightTriangle must satisfy ShapeContract");
    static_assert(!ShapeContract<FlawedShape>, "FlawedShape must NOT satisfy ShapeContract due to noexcept failure!");

    std::cout << "\nStatic assertion confirmed: FlawedShape rejected at compile time due to missing noexcept contract!\n";
    return 0;
}
