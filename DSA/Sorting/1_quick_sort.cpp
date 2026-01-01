#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>

namespace quick_sort
{
    auto partition(auto& arr, std::ptrdiff_t low, std::ptrdiff_t high)
    {
        auto pivot { arr[high] };
        auto pi { low-1 };

        for(auto i{low}; i < high; i++)
        {
            if(arr[i] < pivot)
            {
                pi++;
                std::swap(arr[i], arr[pi]);
            }
        }
        std::swap(arr[pi+1], arr[high]);
        return pi+1;
    }

    void quick_sort(auto& arr, std::ptrdiff_t low, std::ptrdiff_t high)
    {
        if(low < high)
        {
            auto pivot { partition(arr, low, high) };
            quick_sort(arr, low, pivot-1);
            quick_sort(arr, pivot+1, high);
        }
    }
}

void loadVector(auto& arr)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);
    //std::uniform_real_distribution<> dist(1.0, 100.0);

    std::generate(arr.begin(), arr.end(), [&]() 
        { 
            return dist(gen);  
        });
}

void printVector(auto& arr)
{
    for(const auto& el : arr)
        std::cout << el << " ";
    std::cout << "\n";
}

int main()
{
    std::vector<int> arr(10);
    loadVector(arr);

    std::cout << "Before Sorting:\n";
    printVector(arr);

    quick_sort::quick_sort(arr, 0, std::ssize(arr)-1);
    
    std::cout << "\nAfter Sorting:\n";
    printVector(arr);

    return 0;
}