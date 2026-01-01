#include <cstddef>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>

namespace merge_sort 
{
    template <typename T>
    void merge(std::vector<T>& arr, std::ptrdiff_t low, std::ptrdiff_t mid, std::ptrdiff_t high)
    {
        auto left {low};
        auto right {mid+1};

        std::vector<T> temp{};
        temp.reserve(high-low+1);

        while((left <= mid) && (right <= high))
        {
            if(arr[left] <= arr[right])
                temp.push_back(arr[left++]);
            else 
                temp.push_back(arr[right++]);
            
        }

        while(left <= mid)
            temp.push_back(arr[left++]);

        while(right <= high)
            temp.push_back(arr[right++]);

        for(auto i{low}; i <= high; i++)
            arr[i] = temp[i-low];
    }

    template <typename T>
    void merge_sort(std::vector<T>& arr, std::ptrdiff_t low, std::ptrdiff_t high)
    {
        if(low < high)
        {
            auto mid {(low + high)/2};
            merge_sort(arr, low, mid);
            merge_sort(arr, mid+1, high);
            merge(arr, low, mid, high);
        }
    }
}

void loadVector(auto& arr)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

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
    
    std::cout << "Before Sorting.\n";
    printVector(arr);

    merge_sort::merge_sort(arr, 0, std::ssize(arr)-1);

    std::cout << "After Sorting.\n";
    printVector(arr);

    return 0;
}
