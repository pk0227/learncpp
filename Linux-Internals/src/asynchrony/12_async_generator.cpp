/*
 * ASYNC GENERATOR USING C++20 COROUTINES
 * 
 * Advanced example demonstrating coroutines for async iteration pattern.
 * 
 * C++20 COROUTINES:
 * - Functions that can suspend and resume execution
 * - co_yield: Suspends function and returns a value
 * - co_await: Suspends until awaitable is ready  
 * - co_return: Completes the coroutine
 * 
 * COROUTINE INFRASTRUCTURE:
 * - promise_type: Defines coroutine behavior and state
 * - coroutine_handle: Handle to resume/destroy suspended coroutine
 * - suspend_always/never: Control suspension points
 * 
 * ASYNC GENERATOR PATTERN:
 * - Lazily produces values on demand (pull-based)
 * - next(): Resumes coroutine to get next value
 * - value(): Retrieves current value
 * - More memory efficient than generating all values upfront
 * 
 * COMPARISON:
 * - Traditional: Generate all values, store in vector, iterate
 * - Generator: Produce values one at a time, minimal memory
 * 
 * INTERVIEW CONCEPTS:
 * - Difference between generators and regular functions
 * - Stack vs heap (coroutines can have state on heap)
 * - When to use coroutines vs callbacks vs futures
 * 
 * ADVANCED NOTES:
 * - promise_type required by compiler for coroutine functions
 * - Handle cleanup critical (use RAII pattern)
 * - Coroutines more complex than async/await in other languages
 * 
 * REQUIRES: C++20 or later (-std=c++20)
 */

#include <chrono>
#include <coroutine>
#include <iostream>
#include <random>
#include <thread>

template <typename T> class AsyncGenerator {
public:
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  AsyncGenerator(handle_type h) : handle_(h) {}

  ~AsyncGenerator() {
    if (handle_) {
      handle_.destroy();
    }
  }

  AsyncGenerator(const AsyncGenerator &) = delete;
  AsyncGenerator &operator=(const AsyncGenerator &) = delete;

  AsyncGenerator(AsyncGenerator &&other) : handle_(other.handle_) {
    other.handle_ = nullptr;
  }

  AsyncGenerator &operator=(AsyncGenerator &&other) {
    if (this != &other) {
      if (handle_) {
        handle_.destroy();
      }
      handle_ = other.handle_;
      other.handle_ = nullptr;
    }
    return *this;
  }

  bool next() {
    handle_.resume();
    return !handle_.done();
  }

  T value() { return handle_.promise().value_; }

  struct promise_type {
    T value_;

    std::suspend_always initial_suspend() { return {}; }

    std::suspend_always final_suspend() noexcept { return {}; }

    auto get_return_object() {
      return AsyncGenerator{handle_type::from_promise(*this)};
    }

    void return_void() {}

    std::suspend_always yield_value(T value) {
      value_ = value;
      return {};
    }

    void unhandled_exception() { std::exit(1); }
  };

private:
  handle_type handle_;
};

AsyncGenerator<int> async_generator() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.5, 1.5);

  for (int i = 0; i < 5; ++i) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<int>(dis(gen) * 1000)));
    co_yield i;
  }
}

int main() {
  auto gen = async_generator();
  while (gen.next()) {
    std::cout << "Received value: " << gen.value() << std::endl;
  }

  return 0;
}
