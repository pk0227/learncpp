#include <iostream>

void increment(int& value) {  // in-out
    value += 1;
}

int main() {
    int n = 5;
    increment(n);  // n becomes 6

    std::cout << "n : " << n << "\n";
}
