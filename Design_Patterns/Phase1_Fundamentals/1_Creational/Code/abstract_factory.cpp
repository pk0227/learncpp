/**
 * @file abstract_factory.cpp
 * @brief Abstract Factory Pattern
 *
 * THE PROBLEM:
 * You need to create families of related objects (e.g., specific UI buttons and
 * checkboxes for Windows vs Linux vs MacOS) without specifying their concrete
 * classes.
 *
 * THE SOLUTION:
 * - Declare an abstract Factory interface (IGUIFactory).
 * - Create concrete factories for each family (WindowsFactory, LinuxFactory).
 * - Client code works only with the abstract interfaces.
 */

#include <iostream>
#include <memory>
#include <string>

// ===================================
// 1. Abstract Products
// ===================================
class IButton {
public:
  virtual ~IButton() = default; // Virtual destructor is CRITICAL
  virtual void paint() const = 0;
};

class ICheckbox {
public:
  virtual ~ICheckbox() = default;
  virtual void paint() const = 0;
};

// ===================================
// 2. Concrete Products (Family 1: Windows)
// ===================================
class WinButton : public IButton {
public:
  void paint() const override { std::cout << "Rendering Windows Button\n"; }
};

class WinCheckbox : public ICheckbox {
public:
  void paint() const override { std::cout << "Rendering Windows Checkbox\n"; }
};

// ===================================
// 3. Concrete Products (Family 2: Linux)
// ===================================
class LinuxButton : public IButton {
public:
  void paint() const override { std::cout << "Rendering Linux Button\n"; }
};

class LinuxCheckbox : public ICheckbox {
public:
  void paint() const override { std::cout << "Rendering Linux Checkbox\n"; }
};

// ===================================
// 4. Abstract Factory
// ===================================
class IGUIFactory {
public:
  virtual ~IGUIFactory() = default;
  virtual std::unique_ptr<IButton> createButton() = 0;
  virtual std::unique_ptr<ICheckbox> createCheckbox() = 0;
};

// ===================================
// 5. Concrete Factories
// ===================================
class WindowsFactory : public IGUIFactory {
public:
  std::unique_ptr<IButton> createButton() override {
    return std::make_unique<WinButton>();
  }
  std::unique_ptr<ICheckbox> createCheckbox() override {
    return std::make_unique<WinCheckbox>();
  }
};

class LinuxFactory : public IGUIFactory {
public:
  std::unique_ptr<IButton> createButton() override {
    return std::make_unique<LinuxButton>();
  }
  std::unique_ptr<ICheckbox> createCheckbox() override {
    return std::make_unique<LinuxCheckbox>();
  }
};

// ===================================
// 6. Client Code
// ===================================
// Notice: The Application doesn't know about concrete classes (WinButton, etc.)
class Application {
  std::unique_ptr<IButton> button_;
  std::unique_ptr<ICheckbox> checkbox_;

public:
  Application(IGUIFactory &factory) {
    button_ = factory.createButton();
    checkbox_ = factory.createCheckbox();
  }

  void render() {
    button_->paint();
    checkbox_->paint();
  }
};

int main() {
  std::cout << "--- Abstract Factory Demo ---\n";

  // Configuration could come from a file or environment variable
  std::string os_type = "Linux";

  std::unique_ptr<IGUIFactory> factory;

  if (os_type == "Windows") {
    factory = std::make_unique<WindowsFactory>();
  } else {
    factory = std::make_unique<LinuxFactory>();
  }

  Application app(*factory);
  app.render();

  return 0;
}
