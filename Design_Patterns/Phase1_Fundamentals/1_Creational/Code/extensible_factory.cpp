/**
 * @file extensible_factory.cpp
 * @brief Extensible Factory (Self-Registering Plugin Factory)
 *
 * THE PROBLEM:
 * The standard Factory pattern uses a hardcoded switch/if-else:
 *   if (type == "circle")    return new Circle();
 *   if (type == "rectangle") return new Rectangle();
 * Adding a new type forces you to MODIFY the factory. This violates the
 * Open-Closed Principle and is impossible when types come from plugins.
 *
 * THE SOLUTION:
 * Replace the switch with a runtime registry:
 *   map<string, CreatorFn>
 * New types call registerType() — the factory never needs modification.
 * Plugins loaded via dlopen() self-register via static initializers.
 *
 * KEY FEATURES:
 * 1. Runtime registration: factory.registerType("hexagon", []{...})
 * 2. Self-registration: static Registrar objects in each type's .cpp
 * 3. Thread-safe: std::shared_mutex (concurrent reads, exclusive writes)
 * 4. Meyers Singleton: prevents Static Initialization Order Fiasco
 *
 * THREAD SAFETY:
 * - create() uses shared_lock: multiple threads create concurrently
 * - registerType() uses unique_lock: serialized, typically only at startup
 * - No lock held during object construction (creator() outside lock)
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra extensible_factory.cpp -o extensible_factory
 * RUN: ./extensible_factory
 */

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// ============================================================================
// 1. Product Hierarchy (Stable — never changes)
// ============================================================================
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw()        const = 0;
    virtual std::string name() const = 0;
    virtual double area()      const = 0;
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r = 5.0) : radius_(r) {}
    void        draw()        const override { std::cout << "  ○ Circle(r=" << radius_ << ")\n"; }
    std::string name()        const override { return "Circle"; }
    double      area()        const override { return 3.14159 * radius_ * radius_; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w = 4.0, double h = 6.0) : w_(w), h_(h) {}
    void        draw()        const override { std::cout << "  □ Rectangle(" << w_ << "×" << h_ << ")\n"; }
    std::string name()        const override { return "Rectangle"; }
    double      area()        const override { return w_ * h_; }
};

// ============================================================================
// 2. Extensible Factory — Thread-Safe Registry
// ============================================================================
class ShapeFactory {
public:
    using Creator = std::function<std::unique_ptr<Shape>()>;

    // --- Meyers Singleton: thread-safe, prevents SIOF ---
    static ShapeFactory& instance() {
        static ShapeFactory factory;  // Initialized on first call, not at startup
        return factory;
    }

    // --- Register a new type (write path — exclusive lock) ---
    void registerType(std::string name, Creator creator) {
        std::unique_lock lock(mutex_);
        if (registry_.contains(name)) {
            // Allow override in tests (mock registration)
            std::cout << "[Factory] Overriding registration for: " << name << "\n";
        } else {
            std::cout << "[Factory] Registered: " << name << "\n";
        }
        registry_[std::move(name)] = std::move(creator);
    }

    // --- Unregister (for test cleanup — exclusive lock) ---
    bool unregisterType(const std::string& name) {
        std::unique_lock lock(mutex_);
        return registry_.erase(name) > 0;
    }

    // --- Create an instance (read path — shared lock for concurrency) ---
    [[nodiscard]] std::unique_ptr<Shape> create(const std::string& name) const {
        Creator creator;
        {
            // Hold shared lock only to FETCH the creator — not during construction
            std::shared_lock lock(mutex_);
            auto it = registry_.find(name);
            if (it == registry_.end()) {
                throw std::runtime_error("[Factory] Unknown type: '" + name +
                    "'. Did you register it?");
            }
            creator = it->second;  // Copy the std::function (cheap)
        }
        // Construction happens OUTSIDE the lock — allows full concurrency
        return creator();
    }

    // --- Query the registry ---
    bool isRegistered(const std::string& name) const {
        std::shared_lock lock(mutex_);
        return registry_.contains(name);
    }

    std::vector<std::string> registeredTypes() const {
        std::shared_lock lock(mutex_);
        std::vector<std::string> names;
        names.reserve(registry_.size());
        for (const auto& [k, _] : registry_) names.push_back(k);
        return names;
    }

    size_t typeCount() const {
        std::shared_lock lock(mutex_);
        return registry_.size();
    }

private:
    ShapeFactory() = default;

    // No copy or move
    ShapeFactory(const ShapeFactory&) = delete;
    ShapeFactory& operator=(const ShapeFactory&) = delete;

    std::unordered_map<std::string, Creator> registry_;
    mutable std::shared_mutex mutex_;  // Reader-writer lock
};

// ============================================================================
// 3. Self-Registering Registrar
// Construct one of these as a static variable in each type's .cpp file.
// Its constructor fires at static initialization time (before main()).
// ============================================================================
struct ShapeRegistrar {
    ShapeRegistrar(std::string name, ShapeFactory::Creator creator) {
        // Meyers Singleton guarantees factory exists before this runs
        ShapeFactory::instance().registerType(std::move(name), std::move(creator));
    }
};

// --- Auto-registration (would normally be in separate .cpp files) ---
// Simulating what self-registration looks like in a real multi-file project:

static ShapeRegistrar g_circleReg(
    "circle",
    []() { return std::make_unique<Circle>(); }
);

static ShapeRegistrar g_rectangleReg(
    "rectangle",
    []() { return std::make_unique<Rectangle>(); }
);

// ============================================================================
// 4. Plugin Types (Simulated — in real use, these would be in .so files)
// Added at runtime without touching ShapeFactory source code.
// ============================================================================
class Triangle : public Shape {
    double base_, height_;
public:
    Triangle(double b = 6.0, double h = 4.0) : base_(b), height_(h) {}
    void        draw()        const override { std::cout << "  △ Triangle(b=" << base_ << " h=" << height_ << ")\n"; }
    std::string name()        const override { return "Triangle"; }
    double      area()        const override { return 0.5 * base_ * height_; }
};

class Hexagon : public Shape {
    double side_;
public:
    explicit Hexagon(double s = 3.0) : side_(s) {}
    void        draw()        const override { std::cout << "  ⬡ Hexagon(side=" << side_ << ")\n"; }
    std::string name()        const override { return "Hexagon"; }
    double      area()        const override { return 2.598 * side_ * side_; }
};

class MockCircle : public Shape {
public:
    void        draw()        const override { std::cout << "  [MOCK] MockCircle\n"; }
    std::string name()        const override { return "MockCircle"; }
    double      area()        const override { return 0.0; }
};

// ============================================================================
// DEMONSTRATION
// ============================================================================
int main() {
    std::cout << "=== Extensible Factory Pattern Demo ===\n\n";

    // ------------------------------------------------------------------
    // Part 1: Types registered before main() via static initializers
    // ------------------------------------------------------------------
    std::cout << "--- Part 1: Types registered before main() ---\n";
    std::cout << "Registered types: ";
    for (const auto& t : ShapeFactory::instance().registeredTypes()) {
        std::cout << t << " ";
    }
    std::cout << "\n\n";

    // Create using the factory (no type knowledge required by client)
    auto s1 = ShapeFactory::instance().create("circle");
    auto s2 = ShapeFactory::instance().create("rectangle");
    s1->draw();
    s2->draw();

    // ------------------------------------------------------------------
    // Part 2: Runtime Registration (simulating plugin loading)
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 2: Runtime Registration (Plugin Simulation) ---\n";
    std::cout << "[System] Loading plugin: shapes_extended.so...\n";

    // Simulate what dlopen() of a plugin .so would trigger:
    ShapeFactory::instance().registerType("triangle",
        []() { return std::make_unique<Triangle>(); });

    ShapeFactory::instance().registerType("hexagon",
        []() { return std::make_unique<Hexagon>(); });

    std::cout << "\nAll registered types after plugin load: ";
    for (const auto& t : ShapeFactory::instance().registeredTypes()) {
        std::cout << t << " ";
    }
    std::cout << "\n\n";

    // Create all types uniformly — factory code NEVER changed!
    std::vector<std::string> allTypes = {"circle", "rectangle", "triangle", "hexagon"};
    for (const auto& typeName : allTypes) {
        auto shape = ShapeFactory::instance().create(typeName);
        std::cout << "  Created " << shape->name()
                  << " | area=" << shape->area() << " | ";
        shape->draw();
    }

    // ------------------------------------------------------------------
    // Part 3: Error Handling
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 3: Error Handling ---\n";
    try {
        auto unknown = ShapeFactory::instance().create("pentagon");
    } catch (const std::runtime_error& e) {
        std::cout << "  Caught: " << e.what() << "\n";
    }

    std::cout << "  isRegistered('circle'): "
              << std::boolalpha
              << ShapeFactory::instance().isRegistered("circle") << "\n";
    std::cout << "  isRegistered('pentagon'): "
              << ShapeFactory::instance().isRegistered("pentagon") << "\n";

    // ------------------------------------------------------------------
    // Part 4: Test Mock Override (key for unit testing)
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 4: Mock Override (Unit Testing Pattern) ---\n";

    // Override "circle" with a mock for testing
    ShapeFactory::instance().registerType("circle",
        []() { return std::make_unique<MockCircle>(); });

    auto mockShape = ShapeFactory::instance().create("circle");
    mockShape->draw();  // Outputs MockCircle

    // Restore original after test
    ShapeFactory::instance().registerType("circle",
        []() { return std::make_unique<Circle>(); });

    auto realShape = ShapeFactory::instance().create("circle");
    std::cout << "  Restored: ";
    realShape->draw();

    // ------------------------------------------------------------------
    // Part 5: Thread-Safe Concurrent Creation
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 5: Concurrent Creation (Thread Safety Demo) ---\n";

    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<Shape>> results(8);
    std::vector<std::string> types = {"circle","rectangle","triangle","hexagon",
                                       "circle","rectangle","triangle","hexagon"};

    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([&, i]() {
            // Concurrent creates — all use shared_lock, fully parallel
            results[i] = ShapeFactory::instance().create(types[i]);
        });
    }
    for (auto& t : threads) t.join();

    std::cout << "  8 shapes created concurrently:\n";
    for (const auto& shape : results) {
        std::cout << "    " << shape->name()
                  << " (area=" << shape->area() << ")\n";
    }

    // ------------------------------------------------------------------
    // Part 6: Lambda Factory with Parameters
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 6: Parameterized Factory Functions ---\n";

    // Register factories that capture parameters
    ShapeFactory::instance().registerType("big_circle",
        []() { return std::make_unique<Circle>(50.0); });

    ShapeFactory::instance().registerType("small_rect",
        []() { return std::make_unique<Rectangle>(2.0, 1.5); });

    auto big  = ShapeFactory::instance().create("big_circle");
    auto small = ShapeFactory::instance().create("small_rect");
    big->draw();
    small->draw();

    std::cout << "\n  Total registered types: "
              << ShapeFactory::instance().typeCount() << "\n";

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
