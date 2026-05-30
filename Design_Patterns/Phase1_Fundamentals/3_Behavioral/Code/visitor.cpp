/**
 * @file visitor.cpp
 * @brief Visitor Pattern — Classic GoF AND Modern std::variant Approach
 *
 * THE PROBLEM:
 * A shape hierarchy (Circle, Rectangle, Triangle) is stable. But we keep
 * needing new operations: draw, calculateArea, serialize to JSON, export to SVG.
 * Adding each operation as a virtual method forces touching ALL shape classes
 * every time. This violates the Open-Closed Principle.
 *
 * THE SOLUTION:
 * Visitor separates operations from the data hierarchy.
 * - Classic GoF: double dispatch via virtual accept()/visit().
 * - Modern C++17: std::variant + std::visit (compiler-verified exhaustiveness).
 *
 * WHEN TO USE:
 * - Type hierarchy is STABLE (types rarely change).
 * - Operations are GROWING (new algorithms added frequently).
 * - AST traversal, document processing, scene graph operations.
 *
 * THREAD SAFETY:
 * - Read-only Visitors: fully thread-safe for concurrent use.
 * - Accumulating Visitors (totalArea): use one instance per thread,
 *   then merge, OR protect shared state with std::mutex.
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra visitor.cpp -o visitor
 * RUN: ./visitor
 */

#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <cmath>
#include <format>

// ============================================================================
// PART A: Classic GoF Visitor (Double Dispatch)
// ============================================================================

// Forward declarations for double dispatch
class Circle;
class Rectangle;
class Triangle;

// --- Visitor Base Interface ---
class ShapeVisitor {
public:
    virtual ~ShapeVisitor() = default;
    virtual void visit(Circle& c)    = 0;
    virtual void visit(Rectangle& r) = 0;
    virtual void visit(Triangle& t)  = 0;
};

// --- Element Base Interface ---
class Shape {
public:
    virtual ~Shape() = default;
    // The "accept" method is the KEY: it calls v.visit(*this),
    // which resolves the correct overload based on the concrete type.
    virtual void accept(ShapeVisitor& v) = 0;
};

// --- Concrete Elements ---
class Circle : public Shape {
public:
    double radius;
    explicit Circle(double r) : radius(r) {}
    void accept(ShapeVisitor& v) override { v.visit(*this); }
};

class Rectangle : public Shape {
public:
    double width, height;
    Rectangle(double w, double h) : width(w), height(h) {}
    void accept(ShapeVisitor& v) override { v.visit(*this); }
};

class Triangle : public Shape {
public:
    double base, height;
    Triangle(double b, double h) : base(b), height(h) {}
    void accept(ShapeVisitor& v) override { v.visit(*this); }
};

// --- Concrete Visitor 1: Area Calculator ---
class AreaCalculator : public ShapeVisitor {
public:
    double totalArea = 0.0;

    void visit(Circle& c) override {
        totalArea += M_PI * c.radius * c.radius;
    }
    void visit(Rectangle& r) override {
        totalArea += r.width * r.height;
    }
    void visit(Triangle& t) override {
        totalArea += 0.5 * t.base * t.height;
    }

    void report() const {
        std::cout << "  [AreaCalculator] Total Area = " << totalArea << "\n";
    }
};

// --- Concrete Visitor 2: JSON Serializer ---
// (An entirely new operation — Circle/Rectangle/Triangle NOT modified)
class JSONSerializer : public ShapeVisitor {
    std::string output_ = "[";
    bool first_ = true;

    void addComma() {
        if (!first_) output_ += ", ";
        first_ = false;
    }

public:
    void visit(Circle& c) override {
        addComma();
        output_ += "{\"type\":\"Circle\",\"radius\":" + std::to_string(c.radius) + "}";
    }
    void visit(Rectangle& r) override {
        addComma();
        output_ += "{\"type\":\"Rectangle\",\"width\":" + std::to_string(r.width)
                 + ",\"height\":" + std::to_string(r.height) + "}";
    }
    void visit(Triangle& t) override {
        addComma();
        output_ += "{\"type\":\"Triangle\",\"base\":" + std::to_string(t.base)
                 + ",\"height\":" + std::to_string(t.height) + "}";
    }

    std::string getJSON() const { return output_ + "]"; }
};

// --- Concrete Visitor 3: DrawVisitor ---
class DrawVisitor : public ShapeVisitor {
public:
    void visit(Circle& c) override {
        std::cout << "  [Draw] Circle with radius " << c.radius << "\n";
    }
    void visit(Rectangle& r) override {
        std::cout << "  [Draw] Rectangle " << r.width << "x" << r.height << "\n";
    }
    void visit(Triangle& t) override {
        std::cout << "  [Draw] Triangle base=" << t.base
                  << " height=" << t.height << "\n";
    }
};

// ============================================================================
// PART B: Modern C++17 Visitor — std::variant + std::visit
// ============================================================================

// Plain data structs — NO virtual functions, NO accept(), NO base class!
struct VCircle    { double radius; };
struct VRectangle { double width, height; };
struct VTriangle  { double base, height; };

// The type-safe union — holds EXACTLY ONE of the listed types
using VShape = std::variant<VCircle, VRectangle, VTriangle>;

// --- Operation 1: Area (as a callable struct) ---
struct AreaVisitor {
    double operator()(const VCircle& c)    const { return M_PI * c.radius * c.radius; }
    double operator()(const VRectangle& r) const { return r.width * r.height; }
    double operator()(const VTriangle& t)  const { return 0.5 * t.base * t.height; }
};

// --- Operation 2: Name (as a callable struct) ---
struct NameVisitor {
    std::string operator()(const VCircle&)    const { return "Circle"; }
    std::string operator()(const VRectangle&) const { return "Rectangle"; }
    std::string operator()(const VTriangle&)  const { return "Triangle"; }
};

// --- Operation 3: SVG Exporter (as a callable struct) ---
struct SVGExporter {
    std::string operator()(const VCircle& c) const {
        return "<circle r=\"" + std::to_string(c.radius) + "\" fill=\"blue\"/>";
    }
    std::string operator()(const VRectangle& r) const {
        return "<rect width=\"" + std::to_string(r.width)
             + "\" height=\"" + std::to_string(r.height) + "\" fill=\"red\"/>";
    }
    std::string operator()(const VTriangle& t) const {
        return "<polygon points=\"0,0 " + std::to_string(t.base) + ",0 0,"
             + std::to_string(t.height) + "\" fill=\"green\"/>";
    }
};

// ============================================================================
// DEMONSTRATION
// ============================================================================
int main() {
    std::cout << "=== Visitor Pattern Demo ===\n\n";

    // ------------------------------------------------------------------
    // PART A: Classic GoF Visitor
    // ------------------------------------------------------------------
    std::cout << "=== PART A: Classic GoF Double-Dispatch Visitor ===\n";

    std::vector<std::unique_ptr<Shape>> scene;
    scene.push_back(std::make_unique<Circle>(5.0));
    scene.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    scene.push_back(std::make_unique<Triangle>(3.0, 8.0));
    scene.push_back(std::make_unique<Circle>(2.5));

    // Apply DrawVisitor — does not modify any shape class
    std::cout << "\n--- Visitor 1: DrawVisitor ---\n";
    DrawVisitor drawer;
    for (auto& shape : scene) {
        shape->accept(drawer);
    }

    // Apply AreaCalculator — accumulates total
    std::cout << "\n--- Visitor 2: AreaCalculator ---\n";
    AreaCalculator areaCalc;
    for (auto& shape : scene) {
        shape->accept(areaCalc);
    }
    areaCalc.report();

    // Apply JSONSerializer — builds JSON string
    std::cout << "\n--- Visitor 3: JSONSerializer ---\n";
    JSONSerializer jsonSer;
    for (auto& shape : scene) {
        shape->accept(jsonSer);
    }
    std::cout << "  JSON: " << jsonSer.getJSON() << "\n";

    // ------------------------------------------------------------------
    // PART B: Modern std::variant + std::visit
    // ------------------------------------------------------------------
    std::cout << "\n=== PART B: Modern C++17 std::variant + std::visit ===\n";
    std::cout << "(No virtual functions, no accept(), compiler-verified exhaustiveness)\n\n";

    std::vector<VShape> vscene = {
        VCircle{5.0},
        VRectangle{4.0, 6.0},
        VTriangle{3.0, 8.0},
        VCircle{2.5}
    };

    // Operation 1: Name
    std::cout << "--- Types in scene ---\n";
    for (const auto& shape : vscene) {
        std::cout << "  " << std::visit(NameVisitor{}, shape) << "\n";
    }

    // Operation 2: Area
    std::cout << "\n--- Areas ---\n";
    double total = 0.0;
    for (const auto& shape : vscene) {
        double area = std::visit(AreaVisitor{}, shape);
        std::cout << "  " << std::visit(NameVisitor{}, shape)
                  << " area = " << area << "\n";
        total += area;
    }
    std::cout << "  Total area = " << total << "\n";

    // Operation 3: SVG Export
    std::cout << "\n--- SVG Export ---\n";
    for (const auto& shape : vscene) {
        std::cout << "  " << std::visit(SVGExporter{}, shape) << "\n";
    }

    // Lambda visitor (inline, one-off operation)
    std::cout << "\n--- Inline Lambda Visitor ---\n";
    for (const auto& shape : vscene) {
        std::visit([](auto&& s) {
            using T = std::decay_t<decltype(s)>;
            if constexpr (std::is_same_v<T, VCircle>)
                std::cout << "  Circle radius: " << s.radius << "\n";
            else if constexpr (std::is_same_v<T, VRectangle>)
                std::cout << "  Rectangle: " << s.width << "x" << s.height << "\n";
            else if constexpr (std::is_same_v<T, VTriangle>)
                std::cout << "  Triangle base: " << s.base << "\n";
        }, shape);
    }

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
