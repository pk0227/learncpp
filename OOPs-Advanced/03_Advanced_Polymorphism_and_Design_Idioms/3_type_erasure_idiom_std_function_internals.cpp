/**
 * @file 3_type_erasure_idiom_std_function_internals.cpp
 * @brief Demonstrates the Type Erasure Idiom (how std::function / std::any work):
 *        - Storing unrelated types with value semantics in a homogeneous container.
 *        - Tripartite architecture: Concept (abstract base), Model (templated wrapper), External Interface.
 *        - Non-intrusive polymorphism without requiring a shared base class.
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>

// -----------------------------------------------------------------------------
// Completely unrelated classes (Do NOT inherit from any shared base class!)
// -----------------------------------------------------------------------------
class Circle {
public:
    void render() const { std::cout << "  Drawing a Circle (radius = 5.0)\n"; }
};

class Rectangle {
public:
    void render() const { std::cout << "  Drawing a Rectangle (width = 10, height = 20)\n"; }
};

class CustomText {
private:
    std::string m_text;
public:
    CustomText(std::string text) : m_text(std::move(text)) {}
    void render() const { std::cout << "  Drawing Text: \"" << m_text << "\"\n"; }
};

// -----------------------------------------------------------------------------
// The Type Erasure Wrapper: AnyDrawable
// -----------------------------------------------------------------------------
class AnyDrawable {
private:
    // 1. Private Abstract Concept
    struct DrawableConcept {
        virtual ~DrawableConcept() = default;
        virtual void draw() const = 0;
        virtual std::unique_ptr<DrawableConcept> clone() const = 0;
    };

    // 2. Private Templated Model (captures any type T that has .render())
    template <typename T>
    struct DrawableModel : public DrawableConcept {
        T m_object;

        DrawableModel(T obj) : m_object(std::move(obj)) {}

        void draw() const override {
            m_object.render(); // Calls concrete type's method
        }

        std::unique_ptr<DrawableConcept> clone() const override {
            return std::make_unique<DrawableModel<T>>(m_object);
        }
    };

    std::unique_ptr<DrawableConcept> m_pimpl;

public:
    // Templated Constructor: Accepts ANY type that has a .render() method!
    template <typename T>
    AnyDrawable(T object) 
        : m_pimpl(std::make_unique<DrawableModel<T>>(std::move(object))) {}

    // Copy semantics via clone()
    AnyDrawable(const AnyDrawable& other) 
        : m_pimpl(other.m_pimpl ? other.m_pimpl->clone() : nullptr) {}

    AnyDrawable& operator=(const AnyDrawable& other) {
        if (this != &other) {
            m_pimpl = other.m_pimpl ? other.m_pimpl->clone() : nullptr;
        }
        return *this;
    }

    // Move semantics
    AnyDrawable(AnyDrawable&& other) noexcept = default;
    AnyDrawable& operator=(AnyDrawable&& other) noexcept = default;

    // Unified interface
    void render() const {
        if (m_pimpl) {
            m_pimpl->draw();
        }
    }
};

int main() {
    std::cout << "=== Type Erasure with AnyDrawable ===\n";
    std::cout << "Notice that Circle, Rectangle, and CustomText share NO common base class!\n\n";

    // Homogeneous container storing heterogeneous unrelated types!
    std::vector<AnyDrawable> canvas;
    canvas.push_back(Circle{});
    canvas.push_back(Rectangle{});
    canvas.push_back(CustomText{"Modern C++ Type Erasure"});

    for (const auto& item : canvas) {
        item.render(); // Value semantics outside, polymorphism inside!
    }

    return 0;
}
