/**
 * @file decorator.cpp
 * @brief Decorator Pattern
 *
 * SCENARIO:
 * Coffee Shop.
 * Base Coffee: $1.00
 * Add Milk: +$0.50
 * Add Sugar: +$0.20
 *
 * We want to stack them: Coffee + Milk + Sugar.
 */

#include <iostream>
#include <memory>
#include <string>

// ===================================
// 1. Component Interface
// ===================================
class Beverage {
public:
  virtual ~Beverage() = default;
  virtual std::string getDescription() const = 0;
  virtual double cost() const = 0;
};

// ===================================
// 2. Concrete Component
// ===================================
class Espresso : public Beverage {
public:
  std::string getDescription() const override { return "Espresso"; }
  double cost() const override { return 1.00; }
};

// ===================================
// 3. Base Decorator
// ===================================
// It IS A Beverage, and HAS A Beverage
class CondimentDecorator : public Beverage {
protected:
  std::unique_ptr<Beverage> beverage_; // Wraps the inner object

public:
  CondimentDecorator(std::unique_ptr<Beverage> beverage)
      : beverage_(std::move(beverage)) {}
};

// ===================================
// 4. Concrete Decorators
// ===================================
class Milk : public CondimentDecorator {
public:
  Milk(std::unique_ptr<Beverage> b) : CondimentDecorator(std::move(b)) {}

  std::string getDescription() const override {
    return beverage_->getDescription() + ", Milk";
  }

  double cost() const override { return beverage_->cost() + 0.50; }
};

class Sugar : public CondimentDecorator {
public:
  Sugar(std::unique_ptr<Beverage> b) : CondimentDecorator(std::move(b)) {}

  std::string getDescription() const override {
    return beverage_->getDescription() + ", Sugar";
  }

  double cost() const override { return beverage_->cost() + 0.20; }
};

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- Decorator Pattern Demo ---\n";

  // 1. Plain Espresso
  std::unique_ptr<Beverage> coffee = std::make_unique<Espresso>();
  std::cout << coffee->getDescription() << " $" << coffee->cost() << "\n";

  // 2. Add Milk (Transfer ownership of 'coffee' into 'milk')
  coffee = std::make_unique<Milk>(std::move(coffee));
  std::cout << coffee->getDescription() << " $" << coffee->cost() << "\n";

  // 3. Add Sugar
  coffee = std::make_unique<Sugar>(std::move(coffee));
  std::cout << coffee->getDescription() << " $" << coffee->cost() << "\n";

  return 0;
}
