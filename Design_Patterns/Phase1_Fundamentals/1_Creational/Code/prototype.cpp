/**
 * @file prototype.cpp
 * @brief Prototype Pattern — Clone Without Tight Coupling
 *
 * THE PROBLEM:
 * Creating complex objects is expensive (e.g., initializing a fully-parsed
 * config tree, a loaded game entity, or a DB-fetched record). When you need
 * many similar objects, re-running construction logic for each is wasteful.
 *
 * THE SOLUTION:
 * - Define a virtual `clone()` method in the base interface.
 * - Each concrete class knows how to deep-copy itself.
 * - Client just calls `obj->clone()` without knowing the concrete type.
 * - Optionally maintain a "Prototype Registry" for named template objects.
 *
 * KEY C++ DETAIL:
 * `clone()` delegates to the copy constructor. If all members are value types
 * or smart pointers, `= default` copy constructor works. Raw pointers require
 * an explicit deep-copy constructor.
 *
 * THREAD SAFETY:
 * Each clone is a NEW, INDEPENDENT object. No shared mutable state between
 * prototype and clone. Multiple threads can clone concurrently without locks.
 * The shared registry itself should be protected with std::shared_mutex.
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra prototype.cpp -o prototype
 * RUN: ./prototype
 */

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// ============================================================================
// 1. Prototype Interface
// ============================================================================
class Shape {
public:
    virtual ~Shape() = default;

    // The KEY method: polymorphic deep copy through base pointer
    [[nodiscard]] virtual std::unique_ptr<Shape> clone() const = 0;

    virtual void draw() const = 0;
    virtual std::string type() const = 0;
};

// ============================================================================
// 2. Concrete Prototypes
// ============================================================================
class Circle : public Shape {
    double radius_;
    std::string color_;

public:
    Circle(double radius, std::string color)
        : radius_(radius), color_(std::move(color)) {}

    // Explicitly defaulted copy ctor drives the clone (all members are values)
    Circle(const Circle& other) = default;

    [[nodiscard]] std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(*this); // Calls copy constructor
    }

    void draw() const override {
        std::cout << "    Circle | radius=" << radius_
                  << " | color=" << color_ << "\n";
    }

    std::string type() const override { return "Circle"; }

    // Allow modification after cloning for customization
    void setColor(std::string color) { color_ = std::move(color); }
    void setRadius(double r) { radius_ = r; }
};

class Rectangle : public Shape {
    double width_, height_;
    std::string color_;

public:
    Rectangle(double w, double h, std::string color)
        : width_(w), height_(h), color_(std::move(color)) {}

    Rectangle(const Rectangle& other) = default;

    [[nodiscard]] std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Rectangle>(*this);
    }

    void draw() const override {
        std::cout << "    Rectangle | " << width_ << "x" << height_
                  << " | color=" << color_ << "\n";
    }

    std::string type() const override { return "Rectangle"; }
    void setColor(std::string color) { color_ = std::move(color); }
};

class Triangle : public Shape {
    double base_, height_;
    std::string color_;

public:
    Triangle(double base, double height, std::string color)
        : base_(base), height_(height), color_(std::move(color)) {}

    Triangle(const Triangle&) = default;

    [[nodiscard]] std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Triangle>(*this);
    }

    void draw() const override {
        std::cout << "    Triangle | base=" << base_
                  << " height=" << height_
                  << " | color=" << color_ << "\n";
    }

    std::string type() const override { return "Triangle"; }
};

// ============================================================================
// 3. Prototype Registry
// A named cache of template objects. Returns fresh clones, never the original.
// ============================================================================
class ShapeRegistry {
    std::unordered_map<std::string, std::unique_ptr<Shape>> prototypes_;

public:
    void registerPrototype(std::string key, std::unique_ptr<Shape> shape) {
        std::cout << "[Registry] Registered prototype: " << key << "\n";
        prototypes_[std::move(key)] = std::move(shape);
    }

    // Returns a fresh CLONE — the original prototype is never exposed
    [[nodiscard]] std::unique_ptr<Shape> create(const std::string& key) const {
        auto it = prototypes_.find(key);
        if (it == prototypes_.end()) {
            throw std::runtime_error("[Registry] Prototype not found: " + key);
        }
        return it->second->clone();
    }

    bool contains(const std::string& key) const {
        return prototypes_.contains(key);
    }
};

// ============================================================================
// DEMONSTRATION
// ============================================================================
int main() {
    std::cout << "=== Prototype Pattern Demo ===\n\n";

    // ------------------------------------------------------------------
    // Part 1: Basic Cloning and Independence
    // ------------------------------------------------------------------
    std::cout << "--- Part 1: Basic Cloning & Independence ---\n";

    auto original = std::make_unique<Circle>(10.0, "Red");

    auto clone1 = original->clone();
    auto clone2 = original->clone();

    // Modify clones independently — original must be unchanged
    static_cast<Circle*>(clone1.get())->setColor("Blue");
    static_cast<Circle*>(clone2.get())->setRadius(99.0);

    std::cout << "  Original:\n  ";
    original->draw();

    std::cout << "  Clone 1 (color changed to Blue):\n  ";
    clone1->draw();

    std::cout << "  Clone 2 (radius changed to 99.0):\n  ";
    clone2->draw();

    std::cout << "  Original after modifications to clones (must be unchanged):\n  ";
    original->draw();

    // ------------------------------------------------------------------
    // Part 2: Prototype Registry
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 2: Prototype Registry ---\n";

    ShapeRegistry registry;
    registry.registerPrototype("small_circle",
        std::make_unique<Circle>(5.0, "Green"));
    registry.registerPrototype("big_rect",
        std::make_unique<Rectangle>(200.0, 100.0, "Yellow"));
    registry.registerPrototype("ui_triangle",
        std::make_unique<Triangle>(30.0, 20.0, "Purple"));

    std::cout << "\n[Registry] Creating objects from prototypes:\n";
    auto s1 = registry.create("small_circle");
    auto s2 = registry.create("small_circle"); // Another independent copy
    auto s3 = registry.create("big_rect");
    auto s4 = registry.create("ui_triangle");

    s1->draw();
    s2->draw();
    s3->draw();
    s4->draw();

    // ------------------------------------------------------------------
    // Part 3: Polymorphic Clone (Type Preservation)
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 3: Polymorphic Clone (Type Preserved) ---\n";

    std::vector<std::unique_ptr<Shape>> originals;
    originals.push_back(std::make_unique<Circle>(3.0, "Pink"));
    originals.push_back(std::make_unique<Rectangle>(15.0, 8.0, "Cyan"));
    originals.push_back(std::make_unique<Triangle>(12.0, 9.0, "Orange"));

    std::cout << "  Cloned collection (types preserved via virtual clone()):\n";
    for (const auto& shape : originals) {
        auto cloned = shape->clone();
        std::cout << "  Source type: " << shape->type()
                  << " => Cloned as: " << cloned->type() << " => ";
        cloned->draw();
    }

    // ------------------------------------------------------------------
    // Part 4: Error handling
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 4: Error Handling in Registry ---\n";
    try {
        auto invalid = registry.create("nonexistent_shape");
    } catch (const std::runtime_error& e) {
        std::cout << "  Caught: " << e.what() << "\n";
    }

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
