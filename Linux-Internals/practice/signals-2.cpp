#include <iostream>
#include <vector>
#include <thread>
#include <csignal>
#include <unistd.h>
#include <string>
#include <format>
#include <mutex>
#include <chrono>

std::mutex g_mutex{};

void thread_function(int thr_num)
{
    std::thread::id tid { std::this_thread::get_id() };
    std::unique_lock<std::mutex> ul{g_mutex};
    std::cout << "thread - " << thr_num << " (" << tid << ") started\n";
    ul.unlock();
    
    if(thr_num == 5)
    {
        sigset_t set{};
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        
        int sig{};
        
        while(1)
        {
            sigwait(&set, &sig);
            std::cout << "thread - " << thr_num << " (" << tid << ") received signal - " << sig << "\n";
        }
    }
    
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));   
    }
}

int main()
{

    std::thread::id tid { std::this_thread::get_id() };
    std::unique_lock<std::mutex> ul{g_mutex};
    std::cout << "thread - main" << " (" << tid << ") started\n";
    ul.unlock();
    
    sigset_t set{};
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, nullptr);
    
    std::vector<std::thread> vth{};
    
    for(int i{1}; i <= 5; i++)
    {
        vth.emplace_back(thread_function, i);
    }
    
    // sigset_t set{};
    // sigemptyset(&set);
    // sigaddset(&set, SIGINT);
    // pthread_sigmask(SIG_BLOCK, &set, nullptr);
    
    for(auto& th : vth)
        th.join();
    
    return 0;
}