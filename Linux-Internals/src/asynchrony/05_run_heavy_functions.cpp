/*
 * RUNNING HEAVY OPERATIONS WITHOUT BLOCKING UI
 * 
 * Demonstrates how to run multiple long-running tasks asynchronously while
 * maintaining responsive user interaction.
 * 
 * REAL-WORLD SCENARIO:
 * - Backend server processing multiple long-running requests
 * - Desktop application with background tasks and active UI
 * - Data processing pipeline with concurrent user input
 * 
 * ARCHITECTURE:
 * - Main thread: Launches async tasks and waits for results
 * - Worker threads: Execute blocking_function (simulate heavy computation)
 * - Input thread: Handles user input concurrently
 * 
 * KEY TECHNIQUES:
 * - std::async(std::launch::async, ...): Force async execution
 * - std::atomic<bool>: Thread-safe flag for coordinating shutdown
 * - std::vector<std::future<int>>: Store multiple future objects
 * - future.get(): Retrieve results and propagate exceptions
 * 
 * EXCEPTION HANDLING:
 * - Exceptions in async tasks are stored in the future
 * - future.get() rethrows the exception in the calling thread
 * - Try-catch blocks ensure graceful error handling
 * 
 * INTERVIEW INSIGHTS:
 * - How to prevent UI blocking with background tasks
 * - Managing multiple concurrent operations
 * - Proper exception propagation in async code
 */

#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <atomic>

// Function to simulate a heavy computation
int blocking_function(int i) {
  try {
    std::cout << "Running blocking function " << i << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    // Simulate a heavy computation using sleep
    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "Done running blocking function " << i << std::endl;
    return i;
  } catch (const std::exception &e) {
    std::cerr << "Exception in blocking function " << i << ": " << e.what() << '\n';
    return -1;
  }
}

// Function to handle user input
void user_input(std::atomic<bool> &running) {
  std::string user_text;
  while (running) {
    std::cout << "Enter some text: ";
    if (std::getline(std::cin, user_text)) {
      std::cout << "User input: " << user_text << std::endl;
    } else {
      running = false;
    }
  }
}

int main() {
  // Launch 6 heavy tasks asynchronously - they all start running immediately
  std::vector<std::future<int>> blocking_tasks;
  for (int i = 0; i < 6; ++i) {
    blocking_tasks.push_back(
        std::async(std::launch::async, blocking_function, i));
  }

  // Start input thread to handle user interaction while tasks run
  std::atomic<bool> running(true);
  std::thread input_thread(user_input, std::ref(running));

  // Collect results from all tasks
  // get() blocks until each task completes, but tasks run concurrently
  // Total time ≈ 10s (one task), not 60s (6 * 10s) due to parallelism
  std::vector<int> results;
  for (auto &task : blocking_tasks) {
    try {
      results.push_back(task.get());  // Blocks until this specific task finishes
    } catch (const std::exception &e) {
      std::cerr << "Exception in main: " << e.what() << '\n';
      results.push_back(-1);  // Use sentinel value for failed tasks
    }
  }

  std::cout << "Results: ";
  for (const auto &result : results) {
    std::cout << result << " ";
  }
  std::cout << std::endl;

  running = false;
  input_thread.join();

  return 0;
}
