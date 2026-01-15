/**
 * @file strategy.cpp
 * @brief Strategy Pattern
 *
 * SCENARIO:
 * Sorting Context.
 * We want to switch between BubbleSort (for small lists) and QuickSort (for big
 * lists) dynamically at runtime, or let the user choose.
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ===================================
// 1. Storage Strategy Interface
// ===================================
class SortStrategy {
public:
  virtual ~SortStrategy() = default;
  virtual void sort(std::vector<int> &data) = 0;
};

// ===================================
// 2. Concrete Strategies
// ===================================
class BubbleSort : public SortStrategy {
public:
  void sort(std::vector<int> &data) override {
    std::cout << "   [Strategy] Sorting using Bubble Sort (Slow)...\n";
    // Fake implementation
    std::sort(data.begin(), data.end());
  }
};

class QuickSort : public SortStrategy {
public:
  void sort(std::vector<int> &data) override {
    std::cout << "   [Strategy] Sorting using Quick Sort (Fast)...\n";
    std::sort(data.begin(), data.end());
  }
};

// ===================================
// 3. Context
// ===================================
class Sorter {
  std::unique_ptr<SortStrategy> strategy_;

public:
  // Can default to one strategy
  Sorter() : strategy_(std::make_unique<BubbleSort>()) {}

  void setStrategy(std::unique_ptr<SortStrategy> newStrategy) {
    strategy_ = std::move(newStrategy);
  }

  void executeSort(std::vector<int> &data) { strategy_->sort(data); }
};

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- Strategy Pattern Demo ---\n";

  std::vector<int> dataset = {5, 2, 9, 1, 5, 6};
  Sorter sorter;

  // 1. Default (Bubble)
  sorter.executeSort(dataset);

  // 2. Switch to QuickSort
  std::cout << "\n(Switching to QuickSort...)\n";
  sorter.setStrategy(std::make_unique<QuickSort>());

  // 3. New dataset
  dataset = {100, 2, 43, 12};
  sorter.executeSort(dataset);

  return 0;
}
