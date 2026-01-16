/**
 * @file command.cpp
 * @brief Command Pattern - Undo/Redo System for Text Editor
 *
 * SENIOR CONCEPTS:
 * - Command interface with execute/undo
 * - Command history management
 * - Macro commands (composite)
 * - Modern C++ with unique_ptr
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra command.cpp -o command
 * RUN: ./command
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// Receiver: The actual business logic
// ============================================================================
class TextDocument {
private:
  std::string content;

public:
  void insert(const std::string &text, size_t pos) {
    if (pos > content.size())
      pos = content.size();
    content.insert(pos, text);
  }

  void erase(size_t pos, size_t length) {
    if (pos < content.size()) {
      content.erase(pos, length);
    }
  }

  const std::string &getContent() const { return content; }

  void print() const { std::cout << "Document: [" << content << "]\n"; }
};

// ============================================================================
// Command Interface
// ============================================================================
class Command {
public:
  virtual ~Command() = default;
  virtual void execute() = 0;
  virtual void undo() = 0;
  virtual std::string description() const = 0;
};

// ============================================================================
// Concrete Commands
// ============================================================================
class InsertCommand : public Command {
private:
  TextDocument &doc;
  std::string text;
  size_t position;

public:
  InsertCommand(TextDocument &d, std::string t, size_t pos)
      : doc(d), text(std::move(t)), position(pos) {}

  void execute() override { doc.insert(text, position); }

  void undo() override { doc.erase(position, text.length()); }

  std::string description() const override {
    return "Insert '" + text + "' at " + std::to_string(position);
  }
};

class DeleteCommand : public Command {
private:
  TextDocument &doc;
  size_t position;
  size_t length;
  std::string deletedText; // Store for undo

public:
  DeleteCommand(TextDocument &d, size_t pos, size_t len)
      : doc(d), position(pos), length(len) {
    // Capture text before deletion for undo
    const auto &content = doc.getContent();
    if (pos < content.size()) {
      deletedText = content.substr(pos, len);
    }
  }

  void execute() override { doc.erase(position, length); }

  void undo() override { doc.insert(deletedText, position); }

  std::string description() const override {
    return "Delete " + std::to_string(length) + " chars at " +
           std::to_string(position);
  }
};

// ============================================================================
// Macro Command (Composite)
// ============================================================================
class MacroCommand : public Command {
private:
  std::vector<std::unique_ptr<Command>> commands;
  std::string name;

public:
  explicit MacroCommand(std::string n) : name(std::move(n)) {}

  void addCommand(std::unique_ptr<Command> cmd) {
    commands.push_back(std::move(cmd));
  }

  void execute() override {
    for (auto &cmd : commands) {
      cmd->execute();
    }
  }

  void undo() override {
    // Undo in reverse order!
    for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
      (*it)->undo();
    }
  }

  std::string description() const override {
    return "Macro: " + name + " (" + std::to_string(commands.size()) +
           " commands)";
  }
};

// ============================================================================
// Invoker: Command Manager with Undo/Redo
// ============================================================================
class CommandManager {
private:
  std::vector<std::unique_ptr<Command>> history;
  size_t currentPos = 0; // Points to next command to execute

public:
  void execute(std::unique_ptr<Command> cmd) {
    std::cout << "Executing: " << cmd->description() << "\n";

    cmd->execute();

    // Clear redo history (from currentPos onwards)
    history.erase(history.begin() + currentPos, history.end());

    history.push_back(std::move(cmd));
    ++currentPos;
  }

  void undo() {
    if (currentPos == 0) {
      std::cout << "Nothing to undo!\n";
      return;
    }

    --currentPos;
    std::cout << "Undoing: " << history[currentPos]->description() << "\n";
    history[currentPos]->undo();
  }

  void redo() {
    if (currentPos >= history.size()) {
      std::cout << "Nothing to redo!\n";
      return;
    }

    std::cout << "Redoing: " << history[currentPos]->description() << "\n";
    history[currentPos]->execute();
    ++currentPos;
  }

  void printHistory() const {
    std::cout << "\n=== Command History ===\n";
    for (size_t i = 0; i < history.size(); ++i) {
      std::cout << (i < currentPos ? "[X] " : "[ ] ");
      std::cout << history[i]->description() << "\n";
    }
    std::cout << "=======================\n\n";
  }
};

// ============================================================================
// Main: Demonstrate Command Pattern
// ============================================================================
int main() {
  TextDocument doc;
  CommandManager manager;

  std::cout << "=== Text Editor with Undo/Redo ===\n\n";

  // Initial state
  doc.print();

  // Execute commands
  manager.execute(std::make_unique<InsertCommand>(doc, "Hello", 0));
  doc.print();

  manager.execute(std::make_unique<InsertCommand>(doc, " World", 5));
  doc.print();

  manager.execute(
      std::make_unique<DeleteCommand>(doc, 5, 6)); // Delete " World"
  doc.print();

  manager.printHistory();

  // Undo operations
  std::cout << "--- Undoing operations ---\n";
  manager.undo();
  doc.print();

  manager.undo();
  doc.print();

  manager.printHistory();

  // Redo operations
  std::cout << "--- Redoing operations ---\n";
  manager.redo();
  doc.print();

  manager.redo();
  doc.print();

  // Test Macro Command
  std::cout << "\n--- Testing Macro Command ---\n";
  auto macro = std::make_unique<MacroCommand>("Add Title");
  macro->addCommand(std::make_unique<InsertCommand>(doc, "Title: ", 0));
  macro->addCommand(std::make_unique<InsertCommand>(doc, "\n", 7));

  manager.execute(std::move(macro));
  doc.print();

  manager.printHistory();

  manager.undo(); // Undo entire macro
  doc.print();

  std::cout << "\n=== Demo Complete ===\n";
  return 0;
}
