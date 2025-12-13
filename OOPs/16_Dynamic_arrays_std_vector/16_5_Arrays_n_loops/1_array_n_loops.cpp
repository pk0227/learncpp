#include <iostream>
#include <vector>

template <typename T>
T calculateAverage(const std::vector<T>& ref)
{
    T average{};
    for(std::size_t i{}; i < ref.size(); i++)
        average += ref[i];

    average /= static_cast<int>(ref.size());

    return average;
}

int main()
{
    std::vector v1{80, 54, 21, 69, 52};
    std::cout << calculateAverage(v1) << "\n";

    std::vector v2{80.21, 54.114, 21.587, 69.3325, 52.1587};
    std::cout << calculateAverage(v2) << "\n";

    return 0;
}