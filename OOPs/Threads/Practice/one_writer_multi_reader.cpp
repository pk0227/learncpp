#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <chrono>

std::mutex g_m_w_cv{};
std::mutex g_m_r_cv1{};
std::mutex g_m_r_cv2{};
std::condition_variable g_cv{};
std::condition_variable g_cv1{};
std::condition_variable g_cv2{};
bool g_read_ready{false};
int g_count{};
bool all_read{false};
int readers_count{};
bool g_exit{false};
bool g_idle{true};

void Writer(int inc_count)
{
    std::thread::id tid{std::this_thread::get_id()};
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    {
        while(!g_exit)
        {
            std::unique_lock<std::mutex> ul{g_m_w_cv};
            g_cv.wait(ul, []{ return (all_read && g_idle); });
            all_read = false;
            g_idle = false;
            //readers_count = 0;
            
            std::unique_lock<std::mutex> ul2{g_m_r_cv2};
            if(g_count < inc_count)
            {
                g_count++;
                if(g_count == inc_count)
                    g_exit = true;
                g_read_ready = true;
                g_cv2.notify_all();
            }
        }
    }
    
}

void Reader(int thr_num, int total_thrs)
{
    std::thread::id tid{std::this_thread::get_id()};
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    while(!g_exit)
    {
        
        {
            std::unique_lock<std::mutex> ul1{g_m_r_cv1};
            g_cv1.wait(ul1, []{ return g_idle; });
            std::cout << "Thread num : " << thr_num << " , Thread id : " << tid << " ==> Waiting for read...\n";
            readers_count++;
        
            if(readers_count == total_thrs)
            {
                readers_count = 0;
                
                std::unique_lock<std::mutex> ul{g_m_w_cv};
                all_read = true;
                std::cout << "All threads Ready and Waiting...!\n\n\n";
                g_cv.notify_one();
            }
        }
    
        {
            std::unique_lock<std::mutex> ul2{g_m_r_cv2};
            g_cv2.wait(ul2, []{ return g_read_ready; });
            std::cout << "Thread num : " << thr_num << " , Thread id : " << tid << " has read == " << g_count << "\n";
            readers_count++;
            
            if((readers_count == total_thrs) && (!g_exit))
            {
                g_read_ready = false;
                readers_count = 0;
                
                std::unique_lock<std::mutex> ul1{g_m_r_cv1};
                g_idle = true;
                std::cout << "\nInitiating Next Cycle...!\n";
                std::cout << "=======================================================\n";
                g_cv1.notify_all();
            }
        }
    }
}

int main()
{
    const int no_threads{5};
    const int increment_count{10};
    std::vector<std::thread> v_thr{};
    
    for(int i{}; i < no_threads; i++)
    {
        v_thr.emplace_back(Reader, i+1, no_threads);
    }
    
    v_thr.emplace_back(Writer, increment_count);
    
    
    for(auto &th : v_thr)
    {
        if(th.joinable())
            th.join();
    }
    
    std::cout << "\n\nHello World\n";

    return 0;
}