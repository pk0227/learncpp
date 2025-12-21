#include <iostream>
#include <thread>
#include <vector>
#include <numeric>

void accumulate(uint64_t& sum, uint64_t start, uint64_t end)
{
    for(auto i{start}; i < end; i++)
        sum+=i;
}

int main()
{
    int num_threads{1000};
    uint64_t num_elements{1000 * 1000 * 1000};
    std::vector<uint64_t> partial_sum(num_threads);
    std::vector<std::thread> threads{}; 
    
    uint64_t step{num_elements/num_threads};
    
    for(int i{}; i < num_threads; i++)
    {
        threads.push_back(std::thread(accumulate, std::ref(partial_sum[i]), i*step, ((i+1)*step)));
    }

    for(auto& th : threads)
    {
        if(th.joinable())
            th.join();
    }
    
    uint64_t total_sum {std::accumulate(partial_sum.begin(), partial_sum.end(), uint64_t(0))};
    
    std::cout << "total_sum : " << total_sum << "\n";
    
    return 0;
}
