# 🧠 Command Design Pattern
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Command? (Real Meaning)

**Standard Definition:**
"Encapsulate a request as an object, allowing parameterization and queuing of requests."

**Senior Definition:**
*   ✅ **"Reifying Function Calls as First-Class Objects"**
*   ✅ **"The Foundation of Undo/Redo Systems"**
*   ✅ **"Decoupling Invoker from Executor"**

Command turns `object.method(args)` into a standalone object. This allows you to store, queue, log, or reverse operations.

---

## 🔹 2. Modern & Correct C++ Implementation

```cpp
#include <memory>
#include <vector>
#include <iostream>

// Command Interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Receiver (The actual business logic)
class TextEditor {
    std::string text;
public:
    void insertText(const std::string& str, size_t pos) {
        text.insert(pos, str);
    }
    void deleteText(size_t pos, size_t len) {
        text.erase(pos, len);
    }
    const std::string& getText() const { return text; }
};

// Concrete Command
class InsertCommand : public Command {
    TextEditor& editor;
    std:: str;
    size_t position;
public:
    InsertCommand(TextEditor& ed, std::string s, size_t pos) 
        : editor(ed), str(std::move(s)), position(pos) {}
    
    void execute() override {
        editor.insertText(str, position);
    }
    
    void undo() override {
        editor.deleteText(position, str.length());
    }
};

// Invoker (Manages command history)
class CommandManager {
    std::vector<std::unique_ptr<Command>> history;
    size_t current_pos = 0;
public:
    void executeCommand(std::unique_ptr<Command> cmd) {
        cmd->execute();
        // Discard redo history if new command executed
        history.erase(history.begin() + current_pos, history.end());
        history.push_back(std::move(cmd));
        ++current_pos;
    }
    
    void undo() {
        if (current_pos > 0) {
            --current_pos;
            history[current_pos]->undo();
        }
    }
    
    void redo() {
        if (current_pos < history.size()) {
            history[current_pos]->execute();
            ++current_pos;
        }
    }
};
```

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Undo/Redo
*   **The Benefit**: Each command knows how to reverse itself. Store executed commands in a history stack.
*   **Use Case**: Photoshop, Word processors, IDEs.

### ✅ Pro #2: Macro Recording
*   **The Benefit**: Record a sequence of commands, then replay them.
*   **Example**: Excel macros, Vim `.` repeat, Build systems (Makefiles are command patterns).

### ✅ Pro #3: Transactional Operations
*   **The Benefit**: Bundle multiple commands. If one fails, undo all.
*   **Example**: Database transactions (BEGIN, COMMIT, ROLLBACK).

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Class Explosion
*   **The Issue**: Every operation needs its own command class.
*   **Impact**: For a drawing app with 50 tools, you might have 50+ command classes.

### ❌ Con #2: Memory Overhead
*   **The Issue**: Storing undo history means keeping old command objects (and potentially old data states).
*   **Impact**: For an image editor, undoing 100 brush strokes might consume gigabytes.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Command when:
*   Operations are simple function calls with no undo (just call the function directly).
*   Memory is constrained and history is unbounded.

### ✅ Acceptable use cases:
*   **Editors** (text, image, video).
*   **GUI frameworks** (menu items, toolbar buttons).
*   **Game engines** (replay systems, turn-based undo).
*   **Job queues** (task scheduling, thread pools).

---

## 🔹 6. Senior-Level Interview Answer

> "Command pattern decouples the object invoking an operation from the one performing it. I primarily use it for **undo/redo** systems where each command stores enough state to reverse itself. The key tradeoff is memory - storing command history can be expensive, so I often implement a circular buffer with a max history size (e.g., 100 operations). For complex commands, I use the **Memento pattern** to snapshot state rather than duplicating logic in the undo."

---

## 🔹 7. Interview Question Bank

### Q1. Command vs Strategy pattern?
**Answer**: 
- **Command**: Encapsulates a request (what to do + when to do it). Supports undo/queuing.
- **Strategy**: Encapsulates an algorithm (how to do it). Swappable behavior, no undo.

### Q2. How do you handle commands that cannot be undone?
**Answer**: "Mark them as 'irreversible' (boolean flag). When such a command executes, clear the redo stack. Examples: 'Save to disk', 'Send network packet'."

### Q3. What is a Macro Command?
**Answer**: "A composite command that contains multiple sub-commands. When executed, it runs all children in sequence. When undone, it reverses them in reverse order. This is how 'grouped operations' work in editors."

### Q4. How do you optimize memory for undo history?
**Answer**: 
1. **Circular buffer**: Limit history to N commands (e.g., 50).
2. **Delta storage**: Store only the change, not the full state.
3. **Memento pattern**: Snapshot state externally rather than in the command."

### Q5. Can you use lambdas instead of Command classes?
**Answer**: "Yes! In modern C++, `std::function` provides lightweight commands without class explosion:
```cpp
std::vector<std::function<void()>> commands;
commands.push_back([&](){ editor.insertText("Hi", 0); });
```
However, this loses undo capability unless you manually pair lambdas."
