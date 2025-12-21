
#include <chrono>
#include <iostream>
#include <mutex>
/* MUTEX PROTECTION - Fixing race condition
 * Uses mutex to protect shared counter from concurrent access
 * lock_guard or scoped_lock for RAII (exception-safe locking)
 * Ensures only one thread modifies counter at a time
 * Interview: Standard solution for protecting shared state
 */

#include <thread>
#include <vector>

std::mutex mtx;

void foo(int id) {
  mtx.lock(); // comment out this line of code to test what happens without the
              // mutex
  for (int i = 0; i < 100; ++i) {
    std::cout << "Thread id: " << id << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  mtx.unlock(); // comment out this line of code to test what happens without
                // the mutex
}

int main() {
  std::cout << "MAIN FUNCTION STARTS" << std::endl;
  std::vector<std::thread> threads;
  for (int i = 0; i < 4; ++i) {
    threads.emplace_back(foo, i);
  }
  for (auto &thread : threads) {
    thread.join();
  }
  std::cout << "THE END" << std::endl;
  return 0;
}
