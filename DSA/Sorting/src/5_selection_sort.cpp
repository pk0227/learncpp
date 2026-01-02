#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>

namespace selection_sort
{
    void selection_sort(auto& arr)
    {
        auto size { std::ssize(arr) };
        for(auto i{0Z}; i < size-1; i++)
        {
            auto min_index {i};
            for(auto j{i+1}; j < size; j++)
            {
                if(arr[j] < arr[min_index])
                    min_index = j;
            }

            std::swap(arr[i], arr[min_index]);
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

    selection_sort::selection_sort(arr);

    std::cout << "After Sorting\n";
    printVector(arr);

    return 0;
}
