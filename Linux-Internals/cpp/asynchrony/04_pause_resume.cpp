/*
 * PAUSE AND RESUME MECHANISM FOR ASYNC OPERATIONS
 * 
 * Demonstrates advanced thread control using condition variables and atomics.
 * 
 * KEY SYNCHRONIZATION PRIMITIVES:
 * - std::atomic<bool>: Lock-free boolean flags for thread-safe state management
 * - std::mutex: Protects critical sections (though not strictly needed for atomics)
 * - std::condition_variable: Allows threads to wait for specific conditions
 * 
 * ARCHITECTURE:
 * 1. long_function(): Worker thread that can be paused/resumed
 * 2. button_handler(): Input thread that toggles pause state
 * 3. Main thread: Coordinates shutdown
 * 
 * SYNCHRONIZATION PATTERN:
 * - cv.wait(lock, predicate): Atomically releases lock and waits until predicate is true
 * - cv.notify_all(): Wakes all waiting threads to recheck their predicates
 * - Atomic flags prevent data races on shared state
 * 
 * IMPORTANT DETAILS:
 * - std::promise/std::future: Used for clean thread shutdown signaling
 * - Paused thread yields CPU while waiting (doesn't busy-wait)
 * - Lock is released during wait, allowing other threads to modify state
 * 
 * INTERVIEW CONCEPTS:
 * - Condition variables vs busy-waiting (CPU efficiency)
 * - Why we need both mutex AND condition variable
 * - Spurious wakeups and why we use predicate with wait()
 */
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>

std::mutex mtx;
std::condition_variable cv;
std::atomic<bool> paused(false);
std::atomic<bool> running(true);

void long_function(std::promise<void>&& exit_signal) {
  int i = 0;
  while (running) {
    std::unique_lock<std::mutex> lock(mtx);
    // Wait while paused. This atomically:
    // 1. Checks if !paused (predicate)
    // 2. If false (paused is true), releases lock and sleeps
    // 3. When notified, reacquires lock and rechecks predicate
    // 4. Continues only when predicate is true (!paused)
    cv.wait(lock, [] { return !paused; });

    std::cout << "Executing step " << i << std::endl;
    i++;

    lock.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  exit_signal.set_value();
}

void button_handler() {
  while (running) {
    std::cin.get();

    std::unique_lock<std::mutex> lock(mtx);
    paused = !paused;
    cv.notify_all();

    std::string state = paused ? "paused" : "resumed";
    std::cout << "Function " << state << std::endl;
  }
}

int main() {
  std::promise<void> exit_signal;
  std::future<void> future_obj = exit_signal.get_future();
  auto long_function_thread = std::async(std::launch::async, long_function, std::move(exit_signal));
  std::thread button_handler_thread(button_handler);

  std::cout << "Press Enter to pause/resume, or type 'exit' to quit." << std::endl;
  
  std::string input;
  while (true) {
    std::getline(std::cin, input);
    if (input == "exit") {
      running = false;
      paused = false;
      cv.notify_all();
      break;
    }
  }

  future_obj.wait();
  button_handler_thread.join();

  return 0;
}
