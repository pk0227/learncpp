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

void signal_handler(int signum)
{
    std::thread::id tid { std::this_thread::get_id() };
    std::string out {std::format("signal {} caught from signal_handler ({})!\n", signum, tid)};
    write(STDOUT_FILENO, out.c_str(), out.length());
}

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
        pthread_sigmask(SIG_UNBLOCK, &set, nullptr);
        
        //int sig{};
        //sigwait(&set, &sig);      sigwait is alternative to signal handler. Dont use both.
    }
    
    while(1)
    {
        pause();
        std::cout << "pausing again...\n";
    }
}

int main()
{
    struct sigaction sa{};
    sa.sa_handler = signal_handler;
    
    sigaction(SIGINT, &sa, nullptr);
    
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