#include <iostream>
#include <thread>
#include <semaphore>

std::binary_semaphore sem_alpha{1};
std::binary_semaphore sem_numeric{0};

void alpha_func()
{
    for(char i{'A'}; i <= 'Z'; i++)
    {
        sem_alpha.acquire();
        std::cout << i << " - ";
        sem_numeric.release();
    }
    //std::cout << "\n";
}

void numeric_func()
{
    for(char i{'A'}; i <= 'Z'; i++)
    {
        sem_numeric.acquire();
        std::cout << static_cast<int>(i) << "\n";
        sem_alpha.release();
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