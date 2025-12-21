#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

using uli = unsigned long int;

uli g_count = 0;
int readers_done = 0;
const int NUM_READERS = 10;

std::mutex mtx;
std::condition_variable cv;

void Incrementer(int loops)
{
    for (int i = 0; i < loops; ++i)
    {
        std::unique_lock<std::mutex> lock(mtx);

        // Wait until all readers finished previous read
        cv.wait(lock, [] { return readers_done == NUM_READERS; });

        readers_done = 0;
        ++g_count;

        std::cout << "Incremented to " << g_count << "\n";

        cv.notify_all(); // wake readers
    }
}

void Reader(int id, int loops)
{
    for (int i = 0; i < loops; ++i)
    {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [] { return readers_done < NUM_READERS; });

        std::cout << "Reader " << id << " read: " << g_count << "\n";

        ++readers_done;

        if (readers_done == NUM_READERS)
            cv.notify_one(); // wake incrementer
    }
}

int main()
{
    const int loops = 5;

    std::vector<std::thread> readers;
    for (int i = 0; i < NUM_READERS; ++i)
        readers.emplace_back(Reader, i, loops);

    std::thread inc(Incrementer, loops);

    for (auto& t : readers) t.join();
    inc.join();
}
