#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>

namespace bubble_sort 
{
    void bubble_sort(auto& arr)
    {
        auto size { std::ssize(arr) };

        for(auto i{0Z}; i < size-1; i++)
        {
            bool swapped {false};
            for(auto j{0Z}; j < size-1-i; j++)
            {
                if(arr[j] > arr[j+1])
                {
                    std::swap(arr[j], arr[j+1]);
                    swapped = true;
                }       
            }

            if(!swapped)
                break;
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

    std::cout << "Before Sorting\n";
    printVector(arr);

    bubble_sort::bubble_sort(arr);

    std::cout << "Before Sorting\n";
    printVector(arr);

    return 0;
}
