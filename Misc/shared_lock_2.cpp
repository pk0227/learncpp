#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

using uli = unsigned long int;

uli g_count{};
int readers_done{};
int num_readers = 5; // N readers per increment

std::mutex mtx;
std::condition_variable cv_inc, cv_read;

void Incrementer(int max_loop) {
    for (int i = 0; i < max_loop; i++) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            g_count++;
            readers_done = 0;
            std::cout << "Incrementer: " << g_count << "\n";
        }
        cv_read.notify_all(); // wake up all readers

        // wait until all readers finish
        std::unique_lock<std::mutex> lock(mtx);
        cv_inc.wait(lock, [] { return readers_done == num_readers; });
    }
}

void Reader(int id) {
    for (;;) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_read.wait(lock); // wait for incrementer
        std::cout << "Reader " << id << " saw: " << g_count << "\n";
        readers_done++;
        if (readers_done == num_readers) {
            cv_inc.notify_one(); // wake incrementer
        }
    }
}

int main() {
    int max_loop = 10;
    std::vector<std::thread> readers;
    for (int i = 0; i < num_readers; i++) {
        readers.emplace_back(Reader, i);
    }

    std::thread inc(Incrementer, max_loop);

    inc.join();
    // Readers run indefinitely; in real code you'd signal them to stop
    return 0;
}
