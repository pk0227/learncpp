/*
 * MUTEX AND CONDITION VARIABLE IN ASYNC CONTEXT
 * 
 * Demonstrates protecting shared resources in async operations using mutex and
 * condition variables for complex state management.
 * 
 * SYNCHRONIZATION SCENARIO:
 * - Multiple async tasks trying to modify BankAccount concurrently
 * - Only one transfer can be "in progress" at a time
 * - Other transfers must wait their turn
 * 
 * KEY PRIMITIVES:
 * - std::mutex: Mutual exclusion lock for critical sections
 * - std::condition_variable: Allows threads to wait for specific conditions
 * - std::unique_lock: RAII wrapper for mutex with additional features
 * 
 * PATTERN EXPLAINED:
 * 1. unique_lock acquires mutex
 * 2. cv.wait(lock, predicate) releases lock and waits until predicate is true
 * 3. When notified AND predicate true, reacquires lock and continues
 * 4. After operation, notify waiting threads
 * 
 * WHY CONDITION VARIABLE?
 * - Without: Would need busy-waiting (wastes CPU)
 * - With: Thread sleeps until notified (CPU-efficient)
 * 
 * CRITICAL CONCEPT:
 * - condition_variable requires a mutex (can't use alone)
 * - Predicate prevents spurious wakeups
 * - Always check condition in a loop/lambda
 * 
 * INTERVIEW QUESTIONS:
 * - What are spurious wakeups and how to handle them?
 * - Why use unique_lock instead of lock_guard with condition_variable?
 * - What happens if notify is called before wait?
 * - How to avoid deadlocks with multiple mutexes?
 */

#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class BankAccount {
public:
  explicit BankAccount(int balance) : balance_(balance) {}

  void transfer(int amount) {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_var_.wait(lock, [this] { return !transfer_in_progress_; });

    transfer_in_progress_ = true;
    std::cout << "Transferring " << amount << "...\n";
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)); // Simulate some processing time
    balance_ += amount;
    std::cout << "Transfer of " << amount
              << " complete. New balance: " << balance_ << "\n";
    transfer_in_progress_ = false;
    cond_var_.notify_one();
  }

  int balance() const { return balance_; }

private:
  int balance_;
  bool transfer_in_progress_ = false;
  std::mutex mutex_;
  std::condition_variable cond_var_;
};

void transferMoney(BankAccount &account, const std::vector<int> &amounts) {
  for (const auto &amount : amounts) {
    account.transfer(amount);
  }
}

int main() {
  BankAccount account(100);

  auto transfer1 = std::async(std::launch::async, transferMoney,
                              std::ref(account), std::vector<int>{50, -20, 30});
  auto transfer2 =
      std::async(std::launch::async, transferMoney, std::ref(account),
                 std::vector<int>{-10, 60, -10});

  transfer1.wait();
  transfer2.wait();

  std::cout << "Final account balance: " << account.balance() << "\n";

  return 0;
}
