#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex g_mutex{};
std::condition_variable g_cv{};
bool g_alpha{true};
//bool g_numeric{false};

void alpha_func()
{
    
    for(char c{'A'}; c <= 'Z'; c++)
    {
        std::unique_lock<std::mutex> ul{g_mutex};
        g_cv.wait(ul, []{ return g_alpha;});
        std::cout << c << " ";
        g_alpha = false;
        ul.unlock();
        g_cv.notify_one();
    }
    //std::cout << "\n";
}

void numeric_func()
{
    
    for(char c{'A'}; c <= 'Z'; c++)
    {
        std::unique_lock<std::mutex> ul{g_mutex};
        g_cv.wait(ul, []{ return !g_alpha;});
        std::cout << static_cast<int>(c) << "\n";
        g_alpha = true;
        ul.unlock();
        g_cv.notify_one();
    }
    std::cout << "\n";
}

int main()
{
    std::thread th1(alpha_func);
    std::thread th2(numeric_func);
    
    th1.join();
    th2.join();
        
    return 0;
}