# 🧠 Composite Design Pattern
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Composite? (Real Meaning)

**Standard Definition:**
"Compose objects into tree structures to represent part-whole hierarchies."

**Senior Definition:**
*   ✅ **"Treating Individuals and Collections Uniformly"**
*   ✅ **"The Recursive Data Structure Pattern"**
*   ✅ **"Files vs Folders, Widgets vs Containers"**

Composite allows you to call `draw()` on a single Shape or an entire ShapeGroup, and both "just work". The client doesn't need to know if it's dealing with a leaf or a composite.

---

## 🔹 2. Modern & Correct C++ Implementation

```cpp
#include <vector>
#include <memory>
#include <iostream>

// Component Interface
class FileSystemNode {
public:
    virtual ~FileSystemNode() = default;
    virtual void display(int indent = 0) const = 0;
    virtual size_t getSize() const = 0;
};

// Leaf: File
class File : public FileSystemNode {
    std::string name;
    size_t size;
public:
    File(std::string n, size_t s) : name(std::move(n)), size(s) {}
    
    void display(int indent = 0) const override {
        std::cout << std::string(indent, ' ') 
                  << "File: " << name << " (" << size << " bytes)\n";
    }
    
    size_t getSize() const override { return size; }
};

// Composite: Directory
class Directory : public FileSystemNode {
    std::string name;
    std::vector<std::unique_ptr<FileSystemNode>> children;
public:
    explicit Directory(std::string n) : name(std::move(n)) {}
    
    void add(std::unique_ptr<FileSystemNode> node) {
        children.push_back(std::move(node));
    }
    
    void display(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Dir: " << name << "/\n";
        for (const auto& child : children) {
            child->display(indent + 2);
        }
    }
    
    size_t getSize() const override {
        size_t total = 0;
        for (const auto& child : children) {
            total += child->getSize();
        }
        return total;
    }
};
```

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Uniform Treatment
*   **The Benefit**: Client code doesn't need `if (isFile)` vs `if (isDirectory)`.
*   **Example**: `node->getSize()` works for both File (returns file size) and Directory (recursively sums children).

### ✅ Pro #2: Open/Closed Principle
*   **The Benefit**: Adding a new type (e.g., `SymbolicLink`) doesn't break existing code.
*   **Impact**: Easy to extend the hierarchy.

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Type Safety Violation
*   **The Issue**: The `Component` interface often has `add()`/`remove()` methods that only make sense for Composites, not Leaves.
*   **Impact**: Calling `file.add(child)` either throws or no-ops. This violates Interface Segregation Principle.

**Solutions:**
1. **Throw exception** in Leaf's `add()` (runtime check).
2. **Separate interfaces** (`Leaf` vs `Composite` classes) - but loses uniform treatment.

### ❌ Con #2: Overgeneralization
*   **The Issue**: If 90% of operations are different for Leaf vs Composite (e.g., Files have permissions, Directories have listings), the shared interface becomes bloated with `virtual` methods.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Composite when:
*   The hierarchy is shallow (1-2 levels). Don't over-engineer.
*   Leaves and Composites have fundamentally different operations (very few shared behaviors).

### ✅ Acceptable use cases:
*   **File systems** (Files, Directories, Symlinks).
*   **GUI frameworks** (Widgets, Containers, Panels).
*   **Scene graphs** (Game engines - Node, Transform, MeshGroup).
*   **Organizational charts** (Employee, Department).

---

## 🔹 6. Senior-Level Interview Answer

> "Composite is ideal for tree-structured data where you want uniform treatment. I use it for **scene graphs** in rendering engines where a `Node` can be a single Mesh or a TransformGroup containing other Nodes. The key tradeoff is **type safety** - the base class often includes `add()/remove()` which Leaves must either no-op or throw exceptions. I mitigate this by documenting clearly or using `dynamic_cast` checks in debug builds."

---

## 🔹 7. Interview Question Bank

### Q1. Composite vs Decorator?
**Answer**: 
- **Composite**: Tree structure (1-to-many parent-child). Uniform treatment of leaves and branches.
- **Decorator**: Chain/stack (1-to-1 wrapping). Adds behavior dynamically.

### Q2. How do you handle the `add()` problem in Leaves?
**Answer**: Three approaches:
1. **Default implementation**: Leaf's `add()` does nothing (silent failure).
2. **Exception**: Leaf's `add()` throws `std::logic_error("Cannot add to leaf")`.
3. **Separate base classes**: `LeafComponent` (no add) and `CompositeComponent` (has add). Trade uniform interface for safety."

### Q3. Can a Composite have cycles?
**Answer**: "Normally no (tree structure). But if you allow it (graph structure), you need cycle detection to prevent infinite recursion in `display()` or `getSize()`. Track visited nodes with `std::unordered_set`."

### Q4. How do you implement parent pointers?
**Answer**: "Store `Component* parent` in the base class. When `add(child)` is called, set `child->parent = this`. This allows upward traversal. Use `weak_ptr` to avoid ownership cycles."

### Q5. What is the transparency vs safety tradeoff?
**Answer**: 
- **Transparency**: Put `add()/remove()` in Component base. Easy to use, but Leaves can't fulfill the contract (safety violation).
- **Safety**: Separate Leaf and Composite classes. Type-safe, but client must know the difference (loses uniform treatment)."
