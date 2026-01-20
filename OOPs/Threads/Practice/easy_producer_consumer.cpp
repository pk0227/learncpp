#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <condition_variable>

std::mutex g_mutex{};
std::condition_variable g_cv{};

int g_wait_consumer_cnt{};
int g_finish_consumer_cnt{};
bool g_data_ready{false};
int g_shared_data{};

int threads_count{5};

void producer(int times)
{
    for(int i{}; i < times; i++)
    {
        std::unique_lock<std::mutex> ul{g_mutex};
        g_cv.wait(ul, [](){ return (g_wait_consumer_cnt == threads_count); });

        g_shared_data += 10;
        std::cout << "[Producer] : wrote " << g_shared_data << " ...!\n";
        
        g_data_ready = true;
        g_wait_consumer_cnt = 0;

        g_cv.notify_all();

        g_cv.wait(ul, [](){ return !g_data_ready; });
    }
}

void consumer(int times, int thr_num)
{
    for(int i{}; i < times; i++)
    {
        std::unique_lock<std::mutex> ul{g_mutex};
        g_wait_consumer_cnt++;

        if(g_wait_consumer_cnt == threads_count)
        {
            g_cv.notify_all();
        }
    
        int current_data {g_shared_data};
        g_cv.wait(ul, [current_data](){ return (g_data_ready && (current_data != g_shared_data)); });

        std::cout << "\t[consumer - " << thr_num << " ] : read --> " << g_shared_data << "\n";

        g_finish_consumer_cnt++;
        if(g_finish_consumer_cnt == threads_count)
        {
            g_data_ready = false;
            g_finish_consumer_cnt = 0;
            g_cv.notify_all();
        }
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::thread> v_thr{};
    int times{3};

    if(argc > 1)
    {
        threads_count = std::stoi(std::string{argv[1]});
        times = std::stoi(std::string{argv[2]});
    }

    for(int i{}; i < threads_count; i++)
    {
        v_thr.emplace_back(consumer, times, i+1);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    v_thr.emplace_back(producer, times);

    for(auto& thr : v_thr)
        thr.join();

    return 0;
}
