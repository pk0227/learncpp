/**
 * @file 03_lsp_refactored_hierarchy.cpp
 * @brief Demonstrates proper LSP Refactoring: Value Semantics, Segregated Interfaces, & Composition.
 * 
 * Solutions to Classical LSP Violations:
 *   1. Favor Immutability & Value Semantics: When geometric entities are immutable value types,
 *      there are no setters to mutate dimensions, eliminating side-effect contract breaches.
 *   2. Segregate Behaviors: Abstract `Shape` represents queryable geometry (`getArea()`).
 *      Dimension mutations are specific to concrete types or separate mutable interfaces.
 *   3. Interface Segregation for Streams: Split `ReadableStream` from `WritableStream`.
 *      A read-only file only implements `ReadableStream`, making write errors a compile-time
 *      impossibility rather than a runtime crash!
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <concepts>

// ============================================================================
// 1. REFACTORED GEOMETRY: Queryable Shape Contract (Zero Mutable Traps)
// ============================================================================
class Shape {
public:
    virtual ~Shape() = default;
    [[nodiscard]] virtual double getArea() const noexcept = 0;
    [[nodiscard]] virtual std::string describe() const = 0;
};

// Rectangle: Independent dimensions, immutable state
class Rectangle : public Shape {
private:
    double width_;
    double height_;

public:
    Rectangle(double w, double h) noexcept : width_(w), height_(h) {}

    [[nodiscard]] double getWidth() const noexcept { return width_; }
    [[nodiscard]] double getHeight() const noexcept { return height_; }
    [[nodiscard]] double getArea() const noexcept override { return width_ * height_; }

    [[nodiscard]] std::string describe() const override {
        return "Rectangle (" + std::to_string(width_) + " x " + std::to_string(height_) + ")";
    }
};

// Square: Single side invariant, does NOT inherit from Rectangle!
// Subtypes Shape, preserving 100% behavioral substitutability!
class Square : public Shape {
private:
    double side_;

public:
    explicit Square(double s) noexcept : side_(s) {}

    [[nodiscard]] double getSide() const noexcept { return side_; }
    [[nodiscard]] double getArea() const noexcept override { return side_ * side_; }

    [[nodiscard]] std::string describe() const override {
        return "Square (side: " + std::to_string(side_) + ")";
    }
};

// Client depends solely on Shape invariants: Perfectly substitutable!
void renderShapeInfo(const Shape& shape) {
    std::cout << "[Renderer] " << shape.describe() << " | Area: " << shape.getArea() << "\n";
}

// ============================================================================
// 2. REFACTORED STREAMS: Segregated Capabilities (Compile-Time Safety)
// ============================================================================
class ReadableStream {
public:
    virtual ~ReadableStream() = default;
    [[nodiscard]] virtual std::string read(std::size_t bytes) = 0;
};

class WritableStream {
public:
    virtual ~WritableStream() = default;
    virtual void write(const std::string& data) = 0;
};

// ReadWriteFile implements both capabilities
class ReadWriteFile : public ReadableStream, public WritableStream {
public:
    [[nodiscard]] std::string read(std::size_t bytes) override {
        return "[ReadWriteFile] Read " + std::to_string(bytes) + " bytes.";
    }

    void write(const std::string& data) override {
        std::cout << "[ReadWriteFile] Wrote: " << data << "\n";
    }
};

// ReadOnlyFile ONLY implements ReadableStream!
// Writing to it is prevented at COMPILE TIME!
class ReadOnlyFile : public ReadableStream {
public:
    [[nodiscard]] std::string read(std::size_t bytes) override {
        return "[ReadOnlyFile] Secure read " + std::to_string(bytes) + " bytes.";
    }
    // No write() method exists. Zero chance of runtime exception!
};

void performWrite(WritableStream& stream, const std::string& data) {
    stream.write(data);
}

int main() {
    std::cout << "=== LSP Refactored Cleanly: Behavioral Substitutability Preserved ===\n\n";

    // 1. Shapes Demo
    Rectangle rect(10.0, 5.0);
    Square sq(6.0);

    renderShapeInfo(rect);
    renderShapeInfo(sq); // Perfectly valid and behavioral-compliant!

    // 2. Streams Demo
    std::cout << "\n--- Stream Substitutability ---\n";
    ReadWriteFile rwFile;
    ReadOnlyFile roFile;

    performWrite(rwFile, "Audit log data");

    // The following would fail compilation cleanly:
    // performWrite(roFile, "Malicious write"); // error: cannot convert 'ReadOnlyFile' to 'WritableStream&'
    std::cout << roFile.read(32) << "\n";
    std::cout << "Compile-time safety eliminates runtime LSP contract violations!\n";

    return 0;
}
