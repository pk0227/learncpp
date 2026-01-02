#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>

namespace insertion_sort
{
    void insertion_sort(auto& arr)
    {
        auto size { std::ssize(arr) };
        for(auto i{1Z}; i < size; i++)
        {
            auto key {arr[i]};
            auto j {i-1};

            while((j>=0) && (arr[j] > key))
            {
                arr[j+1] = arr[j];
                j--;
            }

            arr[j+1] = key;
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

    insertion_sort::insertion_sort(arr);

    std::cout << "After Sorting\n";
    printVector(arr);

    return 0;
}
