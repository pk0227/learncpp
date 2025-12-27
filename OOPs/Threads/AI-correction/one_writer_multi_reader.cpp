#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

struct Shared {
    std::mutex mtx;
    std::condition_variable cv_ready; // readers rendezvous
    std::condition_variable cv_read;  // start read + wait until all read

    int g_count = 0;
    int ready_count = 0;
    int read_count = 0;

    bool idle = true;        // readers allowed to announce readiness
    bool read_ready = false; // producer has incremented; readers may read
    bool exit = false;
} S;

void Writer(int inc_count, int total_readers)
{
    std::unique_lock<std::mutex> lk(S.mtx);
    while (!S.exit) {
        // Wait until all readers are idle and ready to rendezvous
        S.cv_ready.wait(lk, [] { return S.idle || S.exit; });
        if (S.exit) break;

        // Wait until all readers have announced ready for this cycle
        S.cv_ready.wait(lk, [total_readers] { return S.ready_count == total_readers || S.exit; });
        if (S.exit) break;

        // Start reading phase: increment, reset read_count, flip flags
        S.idle = false;
        S.read_ready = true;
        ++S.g_count;
        std::cout << "\n[Writer] g_count = " << S.g_count << "\n";

        if (S.g_count >= inc_count) {
            S.exit = true;
            // Wake everyone so they can exit
            S.cv_read.notify_all();
            S.cv_ready.notify_all();
            break;
        }

        S.read_count = 0;
        // Let readers proceed to read
        S.cv_read.notify_all();

        // Wait until all readers finish reading
        S.cv_read.wait(lk, [total_readers] { return S.read_count == total_readers || S.exit; });
        if (S.exit) break;

        // Prepare next cycle: allow readers to rendezvous again
        S.read_ready = false;
        S.ready_count = 0;
        S.idle = true;
        S.cv_ready.notify_all();
    }
}

void Reader(int id, int total_readers)
{
    std::unique_lock<std::mutex> lk(S.mtx);
    while (true) {
        // Phase 1: announce ready when idle is true
        S.cv_ready.wait(lk, [] { return S.idle || S.exit; });
        if (S.exit) break;

        ++S.ready_count;
        std::cout << "[Reader " << id << "] ready\n";
        if (S.ready_count == total_readers) {
            // Wake writer to start read phase
            S.cv_ready.notify_one();
        }

        // Phase 2: wait until writer sets read_ready
        S.cv_read.wait(lk, [] { return S.read_ready || S.exit; });
        if (S.exit) break;

        std::cout << "[Reader " << id << "] read " << S.g_count << "\n";
        ++S.read_count;
        if (S.read_count == total_readers) {
            // wake writer: all read completed
            S.cv_read.notify_one();
        }
    }
    std::cout << "[Reader " << id << "] exit\n";
}

int main()
{
    const int no_threads = 5;
    const int increment_count = 10;

    std::vector<std::thread> readers;
    for (int i = 0; i < no_threads; ++i) {
        readers.emplace_back(Reader, i + 1, no_threads);
    }
    std::thread writer(Writer, increment_count, no_threads);

    for (auto& t : readers) t.join();
    writer.join();

    std::cout << "\nDone.\n";
    return 0;
}
