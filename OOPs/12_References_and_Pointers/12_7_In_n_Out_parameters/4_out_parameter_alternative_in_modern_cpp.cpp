#include <iostream>
#include <utility>  // for std::pair

// Function returning a pair of integers
std::pair<int, int> getCoordinates() {
    return {10, 20};  // returns a temporary std::pair<int, int>
}

int main() {
    // Structured binding: automatically unpacks the pair into x and y
    auto [x, y] = getCoordinates();

    std::cout << "x = " << x << '\n';
    std::cout << "y = " << y << '\n';

    return 0;
}
