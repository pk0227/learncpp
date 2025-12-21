/*
 * INTER-TASK COMMUNICATION USING PROMISE AND FUTURE
 * 
 * Demonstrates the producer-consumer pattern using std::promise and std::future
 * for one-time data transfer between asynchronous tasks.
 * 
 * KEY CONCEPTS:
 * - std::promise<T>: The "write" end - allows setting a value for future retrieval
 * - std::future<T>: The "read" end - allows retrieving the value set by promise
 * - One-time communication: promise can only be set once, future can only be retrieved once
 * 
 * COMMUNICATION FLOW:
 * 1. Create promise/future pair
 * 2. Producer sets value via promise.set_value()
 * 3. Consumer blocks on future.get() until value is available
 * 4. Once get() is called, the value is moved out (future becomes invalid)
 * 
 * PROMISE/FUTURE vs QUEUE:
 * - Promise/Future: One-time, single value transfer
 * - Queue: Multiple values, continuous communication
 * 
 * THREAD SYNCHRONIZATION:
 * - future.get() automatically blocks until promise.set_value() is called
 * - No explicit mutex/cv needed for this simple case
 * - Exception-safe: if promise is destroyed without set_value(), future.get() throws
 * 
 * INTERVIEW POINTS:
 * - How promise/future differ from callback functions
 * - Why this is cleaner than shared variables with mutex
 * - Limitation: Can't reuse the same promise/future pair
 */

#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <thread>

int produce_data() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(1, 100);
  int data = distr(gen);
  std::cout << "Produced data: " << data << std::endl;
  return data;
}

void consume_data(int data) {
  std::cout << "Consumed data: " << data << std::endl;
}

int main() {
  std::promise<int> data_promise;
  std::future<int> data_future = data_promise.get_future();

  // Producer task
  auto producer_task = std::async(std::launch::async, [&data_promise]() {
    int data = produce_data();
    data_promise.set_value(data);
  });

  // Consumer task
  auto consumer_task = std::async(std::launch::async, [&data_future]() {
    int data = data_future.get();
    consume_data(data);
  });

  producer_task.wait();
  consumer_task.wait();

  return 0;
}
