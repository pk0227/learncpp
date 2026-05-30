/**
 * @file flyweight.cpp
 * @brief Flyweight Pattern — Sharing Intrinsic State Across Massive Populations
 *
 * THE PROBLEM:
 * A game renders 1,000,000 trees. Each tree has a name, texture, and 3D model
 * (heavy data), plus a position (lightweight). Storing all data per-object
 * wastes enormous RAM (2GB+ for texture data alone).
 *
 * THE SOLUTION (Intrinsic vs Extrinsic split):
 * - INTRINSIC state (shared, immutable): TreeType {name, texture, color}.
 *   Created ONCE per unique type, shared by all trees of that type.
 * - EXTRINSIC state (unique, lightweight): Tree {x, y, ptr to TreeType}.
 *   Stored per object, but minimal in size.
 *
 * MEMORY COMPARISON:
 * Without Flyweight: 1,000,000 trees × 2MB texture data = 2,000 GB
 * With    Flyweight: 3 tree types × 2MB = 6MB + 1,000,000 × 16 bytes = ~22MB
 *
 * THREAD SAFETY:
 * - Intrinsic (shared) objects are IMMUTABLE → zero-lock reads.
 * - The Factory's write path (new type creation) needs a write lock.
 *   Use std::shared_mutex for maximum concurrent read performance.
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra flyweight.cpp -o flyweight
 * RUN: ./flyweight
 */

#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>

// ============================================================================
// 1. Intrinsic State — Shared, Immutable, Heavy
// ============================================================================
struct TreeType {
    const std::string name;
    const std::string color;
    const std::string texture; // Represents heavy data (model, mesh, etc.)

    TreeType(std::string n, std::string c, std::string t)
        : name(std::move(n)), color(std::move(c)), texture(std::move(t)) {}

    // Intrinsic objects are IMMUTABLE. No setters.
    void render(int x, int y) const {
        std::cout << "  [" << name << "] color=" << color
                  << " texture=" << texture
                  << " at (" << x << ", " << y << ")\n";
    }
};

// ============================================================================
// 2. Flyweight Factory — Caches Intrinsic State
// Thread-safe: shared_mutex allows concurrent reads, exclusive writes.
// ============================================================================
class TreeFactory {
    std::unordered_map<std::string, std::shared_ptr<TreeType>> cache_;
    mutable std::shared_mutex mutex_;  // Reader-writer lock

public:
    std::shared_ptr<TreeType> getTreeType(
        const std::string& name,
        const std::string& color,
        const std::string& texture)
    {
        std::string key = name + "|" + color + "|" + texture;

        // --- Hot path: read (shared lock, multiple threads allowed) ---
        {
            std::shared_lock lock(mutex_);
            auto it = cache_.find(key);
            if (it != cache_.end()) {
                return it->second; // Cache HIT — no allocation
            }
        }

        // --- Cold path: write (exclusive lock, one thread at a time) ---
        std::unique_lock lock(mutex_);
        // Double-check after acquiring write lock (another thread may have inserted)
        if (!cache_.contains(key)) {
            std::cout << "[Factory] Creating new TreeType: " << name << "\n";
            cache_[key] = std::make_shared<TreeType>(name, color, texture);
        }
        return cache_[key];
    }

    size_t uniqueTypesCount() const {
        std::shared_lock lock(mutex_);
        return cache_.size();
    }
};

// ============================================================================
// 3. Extrinsic State — Unique Per Object, Lightweight
// ============================================================================
struct Tree {
    int x, y;                               // Unique per tree (extrinsic)
    std::shared_ptr<const TreeType> type;   // Shared intrinsic reference

    Tree(int x, int y, std::shared_ptr<const TreeType> type)
        : x(x), y(y), type(std::move(type)) {}

    void render() const {
        type->render(x, y);  // Extrinsic state passed as context
    }
};

// ============================================================================
// 4. Context / Forest — Holds all the lightweight Tree objects
// ============================================================================
class Forest {
    std::vector<Tree> trees_;
    TreeFactory factory_;

public:
    void plantTree(int x, int y,
                   const std::string& name,
                   const std::string& color,
                   const std::string& texture)
    {
        // Get (or create once) the shared intrinsic state
        auto type = factory_.getTreeType(name, color, texture);
        trees_.emplace_back(x, y, std::move(type));
    }

    void renderAll() const {
        std::cout << "[Forest] Rendering " << trees_.size() << " trees:\n";
        for (const auto& tree : trees_) {
            tree.render();
        }
    }

    void printMemoryReport() const {
        std::cout << "\n[Memory Report]\n";
        std::cout << "  Total trees planted: " << trees_.size() << "\n";
        std::cout << "  Unique TreeType objects in memory: "
                  << factory_.uniqueTypesCount() << "\n";
        std::cout << "  Each Tree struct size: " << sizeof(Tree) << " bytes\n";
        std::cout << "  Total extrinsic memory: "
                  << trees_.size() * sizeof(Tree) << " bytes\n";
        std::cout << "  (Intrinsic heavy data shared — NOT multiplied per tree)\n";
    }
};

// ============================================================================
// DEMONSTRATION
// ============================================================================
int main() {
    std::cout << "=== Flyweight Pattern Demo: Forest of 1 Million Trees ===\n\n";

    Forest forest;

    // ------------------------------------------------------------------
    // Part 1: Small demo - show type sharing
    // ------------------------------------------------------------------
    std::cout << "--- Part 1: Planting Trees (showing type reuse) ---\n";

    // 10 trees across only 3 tree types
    // Factory will only create 3 TreeType objects regardless of tree count
    forest.plantTree(10, 20, "Oak",   "Dark Green", "oak_texture.png");
    forest.plantTree(30, 50, "Pine",  "Light Green","pine_texture.png");
    forest.plantTree(50, 10, "Birch", "White",      "birch_texture.png");
    forest.plantTree(70, 80, "Oak",   "Dark Green", "oak_texture.png"); // Reuses Oak!
    forest.plantTree(90, 30, "Pine",  "Light Green","pine_texture.png"); // Reuses Pine!
    forest.plantTree(15, 45, "Oak",   "Dark Green", "oak_texture.png"); // Reuses Oak!
    forest.plantTree(35, 65, "Birch", "White",      "birch_texture.png"); // Reuses Birch!
    forest.plantTree(55, 85, "Pine",  "Light Green","pine_texture.png"); // Reuses Pine!
    forest.plantTree(75, 25, "Oak",   "Dark Green", "oak_texture.png"); // Reuses Oak!
    forest.plantTree(95, 55, "Birch", "White",      "birch_texture.png"); // Reuses Birch!

    forest.renderAll();
    forest.printMemoryReport();

    // ------------------------------------------------------------------
    // Part 2: Conceptual Large-Scale Demo
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 2: Conceptual Large-Scale Impact ---\n";

    // These numbers simulate what would happen with 1,000,000 trees
    constexpr size_t TREE_COUNT = 1'000'000;
    constexpr size_t UNIQUE_TYPES = 3;
    constexpr size_t TEXTURE_SIZE_MB = 2;     // 2MB per texture
    constexpr size_t EXTRINSIC_BYTES = 24;    // ~24 bytes per Tree (x,y,ptr)

    size_t without_flyweight_mb = (TREE_COUNT * TEXTURE_SIZE_MB);
    size_t with_flyweight_mb    = (UNIQUE_TYPES * TEXTURE_SIZE_MB)
                                  + (TREE_COUNT * EXTRINSIC_BYTES) / (1024*1024);

    std::cout << "  Simulated forest: " << TREE_COUNT << " trees, "
              << UNIQUE_TYPES << " unique types\n";
    std::cout << "  RAM without Flyweight: ~"
              << without_flyweight_mb << " MB (per-object texture copy)\n";
    std::cout << "  RAM with Flyweight:    ~"
              << with_flyweight_mb << " MB (shared textures + lightweight trees)\n";
    std::cout << "  Memory saved: ~"
              << (without_flyweight_mb - with_flyweight_mb) << " MB\n";

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
