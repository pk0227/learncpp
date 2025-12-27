#include <chrono>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

// Semaphores for synchronization
std::binary_semaphore sem_writer_wait{
    0}; // Writer waits for all readers to be ready
std::counting_semaphore<5> sem_readers_wait{
    5}; // Readers wait in idle state (initially all can proceed)
std::counting_semaphore<5> sem_read_ready{
    0}; // Readers wait for data to be ready

// Mutexes for protecting shared state
std::mutex g_mutex{};
int g_count{};
int readers_count{};
bool g_exit{false};
const int NO_THREADS{5};

void Writer(int inc_count) {
  std::thread::id tid{std::this_thread::get_id()};
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  while (!g_exit) {
    // Wait for all readers to signal they're ready
    sem_writer_wait.acquire();

    // Critical section: update shared data
    {
      std::lock_guard<std::mutex> lg{g_mutex};
      if (g_count < inc_count) {
        g_count++;
        if (g_count == inc_count)
          g_exit = true;
      }
    }

    // Signal all readers that data is ready
    for (int i = 0; i < NO_THREADS; i++) {
      sem_read_ready.release();
    }
  }
}

void Reader(int thr_num, int total_thrs) {
  std::thread::id tid{std::this_thread::get_id()};
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  while (!g_exit) {
    // Wait for idle state (can enter waiting phase)
    sem_readers_wait.acquire();

    // Signal readiness and check if all readers are ready
    {
      std::lock_guard<std::mutex> lg{g_mutex};
      std::cout << "Thread num : " << thr_num << " , Thread id : " << tid
                << " ==> Waiting for read...\n";
      readers_count++;

      if (readers_count == total_thrs) {
        readers_count = 0;
        std::cout << "All threads Ready and Waiting...!\n\n\n";

        // Signal writer that all readers are ready
        sem_writer_wait.release();
      }
    }

    // Wait for data to be ready
    sem_read_ready.acquire();

    // Read the data
    {
      std::lock_guard<std::mutex> lg{g_mutex};
      std::cout << "Thread num : " << thr_num << " , Thread id : " << tid
                << " has read == " << g_count << "\n";
      readers_count++;

      if ((readers_count == total_thrs) && (!g_exit)) {
        readers_count = 0;
        std::cout << "\nInitiating Next Cycle...!\n";
        std::cout
            << "=======================================================\n";

        // Release all readers to start next cycle
        for (int i = 0; i < total_thrs; i++) {
          sem_readers_wait.release();
        }
      }
    }
  }
}

int main() {
  const int no_threads{5};
  const int increment_count{10};
  std::vector<std::thread> v_thr{};

  for (int i{}; i < no_threads; i++) {
    v_thr.emplace_back(Reader, i + 1, no_threads);
  }

  v_thr.emplace_back(Writer, increment_count);

  for (auto &th : v_thr) {
    if (th.joinable())
      th.join();
  }

  std::cout << "\n\nHello World\n";

  return 0;
}