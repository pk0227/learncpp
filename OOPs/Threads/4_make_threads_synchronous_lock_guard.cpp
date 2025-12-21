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
    //std::lock_guard<std::mutex> guard{g_mutex};
    for(int i{}; i < 100; i++)
    {
        std::lock_guard<std::mutex> guard{g_mutex};
        g_count++;
    }
}

int main()
{
    std::vector<std::thread> thr_v{};
    int max_loop{100};
    int num_thrs{100};
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
        std::cout << i << " : " << g_count << " \n";
    }

    assert(m[max_loop] == max_loop);
    std::cout << "\n\nm[max_loop] : " << m[max_loop] << "\n";

    return 0;
}