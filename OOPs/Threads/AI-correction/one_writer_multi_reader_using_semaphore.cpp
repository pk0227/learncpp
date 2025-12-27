#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <semaphore>
#include <chrono>

constexpr int g_no_of_threads{5};
std::atomic<bool> g_exit{false};

std::binary_semaphore producer_wait{0};                  // producer waits for N ready
std::counting_semaphore<g_no_of_threads> ready_gate{g_no_of_threads}; // N consumers can declare ready
std::counting_semaphore<g_no_of_threads> start_read{0};  // producer releases N permits to read

std::mutex mtx;
int g_data{0};
int ready_count{0};
int read_count{0};

void producer(int no_of_cycles)
{
    for (int cycle = 0; cycle < no_of_cycles; ++cycle) {
        // Wait until all consumers are ready for this cycle
        producer_wait.acquire();

        {
            std::lock_guard<std::mutex> lock(mtx);
            ++g_data;
            std::cout << "\n[Producer] incremented to " << g_data << " (cycle " << cycle+1 << ")\n";
            // reset read_count for this cycle
            read_count = 0;
        }

        // Allow all consumers to read
        for (int i = 0; i < g_no_of_threads; ++i) start_read.release();

        // Wait until all consumers finish reading
        // We can piggyback on ready_gate refilling: consumers will refill ready_gate after all read.
        // Alternatively, use another binary semaphore. Here we spin on a condition under sleep for brevity.
        for (;;) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::lock_guard<std::mutex> lock(mtx);
            if (read_count == g_no_of_threads) break;
        }
    }

    // Signal exit and unblock any waiters
    g_exit.store(true, std::memory_order_relaxed);

    // Drain semaphores to ensure all consumers can leave cleanly
    for (int i = 0; i < g_no_of_threads; ++i) {
        start_read.release();
        ready_gate.release();
    }

    std::cout << "\n[Producer] completed " << no_of_cycles << " cycles. Exiting.\n";
}

void consumer(int id)
{
    while (true) {
        // Phase 1: announce ready for next cycle
        ready_gate.acquire();
        if (g_exit.load(std::memory_order_relaxed)) break;

        bool notify_producer = false;
        {
            std::lock_guard<std::mutex> lock(mtx);
            ++ready_count;
            std::cout << "[Consumer " << id << "] ready\n";
            if (ready_count == g_no_of_threads) {
                // reset for next cycle
                ready_count = 0;
                notify_producer = true;
            }
        }
        if (notify_producer) {
            producer_wait.release();
        }

        // Phase 2: wait to read
        start_read.acquire();
        if (g_exit.load(std::memory_order_relaxed)) break;

        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "[Consumer " << id << "] read " << g_data << "\n";
            ++read_count;
            if (read_count == g_no_of_threads) {
                // All finished reading; refill ready_gate for the next cycle
                for (int i = 0; i < g_no_of_threads; ++i) ready_gate.release();
                std::cout << "---- next cycle ----\n";
            }
        }
    }

    std::cout << "[Consumer " << id << "] exit\n";
}

int main()
{
    std::vector<std::thread> threads;
    threads.emplace_back(producer, 10);
    for (int i = 0; i < g_no_of_threads; ++i) {
        threads.emplace_back(consumer, i + 1);
    }

    for (auto& t : threads) t.join();
    std::cout << "Done.\n";
    return 0;
}
