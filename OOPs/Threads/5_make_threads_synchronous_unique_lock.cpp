#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <cassert>
#include <mutex>

using uli = unsigned long int;

uli g_count{};
std::mutex g_mutex{};

void do_some_work()
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::unique_lock<std::mutex> ul{g_mutex};
    for(int i{}; i < 10; i++)
    {
        //std::unique_lock<std::mutex> ul{g_mutex};     // ul scope is limited to for-loop block only
        g_count++;
    }
        ul.unlock();
        std::cout << "thread_id : " << tid << "\n";     // prints asynchronously 
        ul.lock();
}

int main()
{
    std::vector<std::thread> thr_v{};
    int max_loop{10};
    int num_thrs{10};
    std::map<int, int> m{};

    for(int i{}; i < max_loop; i++)
    {
        g_count = 0;
        for(int thr_c{}; thr_c < num_thrs; thr_c++)
        {
            
            thr_v.push_back(std::thread(do_some_work));
        }

        for(auto& th : thr_v)
        {
            if(th.joinable())
                th.join();
        }

        m[max_loop]++;
        std::cout << "===============================\n";
        std::cout << i << " : " << g_count << " \n";
        std::cout << "===============================\n";
    }

    assert(m[max_loop] == max_loop);
    std::cout << "\n\nm[max_loop] : " << m[max_loop] << "\n";

    return 0;
}