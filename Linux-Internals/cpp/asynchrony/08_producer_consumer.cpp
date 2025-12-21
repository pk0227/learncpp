/*
 * PRODUCER-CONSUMER PATTERN WITH ASYNC OPERATIONS
 * 
 * Classic concurrency pattern demonstrating thread-safe queue for async task coordination.
 * 
 * PATTERN OVERVIEW:
 * - Producers: Generate items and add them to shared queue
 * - Consumers: Remove items from queue and process them
 * - Queue: Thread-safe buffer between producers and consumers
 * 
 * THREAD SYNCHRONIZATION:
 * - std::mutex: Protects queue from concurrent access
 * - std::condition_variable: Signals when items are available
 * - Consumers block when queue is empty (instead of busy-waiting)
 * 
 * KEY COMPONENTS:
 * - AsyncQueue class: Thread-safe FIFO queue
 * - put(): Adds item, notifies one waiting consumer
 * - get(): Blocks until item available, then removes and returns it
 * 
 * ADVANTAGES OF THIS PATTERN:
 * - Decouples production from consumption (different rates OK)
 * - Buffering smooths out rate differences
 * - Scalable: Can add more producers/consumers independently
 * - CPU-efficient: Threads sleep when idle (no busy-waiting)
 * 
 * COMMON INTERVIEW QUESTIONS:
 * - How to handle queue shutdown gracefully?
 * - What if queue grows unbounded? (add max size limit)
 * - Difference between notify_one() vs notify_all()
 * - How to prevent deadlocks in this pattern?
 * 
 * REAL-WORLD APPLICATIONS:
 * - Task scheduling systems
 * - Message passing between services
 * - Request handling in web servers
 * - Log aggregation systems
 */

#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

class AsyncQueue {
public:
  // Add item to queue (thread-safe)
  void put(const std::string &item) {
    std::unique_lock<std::mutex> lock(m_mutex);  // Acquire exclusive access
    m_queue.push(item);                          // Add to queue
    m_condition.notify_one();                    // Wake one waiting consumer
  }  // Lock automatically released here

  // Remove and return item from queue (blocks if empty)
  std::string get() {
    std::unique_lock<std::mutex> lock(m_mutex);
    // Wait until queue is not empty (handles spurious wakeups)
    m_condition.wait(lock, [&] { return !m_queue.empty(); });
    std::string item = m_queue.front();  // Get first item
    m_queue.pop();                       // Remove it
    return item;                         // Return (lock released automatically)
  }

private:
  std::queue<std::string> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_condition;
};

void producer(std::shared_ptr<AsyncQueue> queue, int producer_id) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(500, 2000);

  for (int i = 0; i < 5; ++i) {
    std::string item = "Item " + std::to_string(i) + " from producer " +
                       std::to_string(producer_id);
    queue->put(item);
    std::cout << "Producer " << producer_id << " produced " << item
              << std::endl;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<int>(dis(gen))));
  }
}

void consumer(std::shared_ptr<AsyncQueue> queue, int consumer_id) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(500, 2000);

  for (int i = 0; i < 5; ++i) {
    std::string item = queue->get();
    std::cout << "Consumer " << consumer_id << " consumed " << item
              << std::endl;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<int>(dis(gen))));
  }
}

int main() {
  auto queue = std::make_shared<AsyncQueue>();

  std::vector<std::future<void>> producers;
  for (int i = 0; i < 3; ++i) {
    producers.push_back(std::async(std::launch::async, producer, queue, i));
  }

  std::vector<std::future<void>> consumers;
  for (int i = 0; i < 2; ++i) {
    consumers.push_back(std::async(std::launch::async, consumer, queue, i));
  }

  for (auto &producer_future : producers) {
    try {
      producer_future.get(); // Ensure exceptions are propagated
    } catch (const std::exception &e) {
      std::cerr << "Exception in producer: " << e.what() << '\n';
    }
  }

  for (auto &consumer_future : consumers) {
    try {
      consumer_future.get(); // Ensure exceptions are propagated
    } catch (const std::exception &e) {
      std::cerr << "Exception in consumer: " << e.what() << '\n';
    }
  }

  return 0;
}
