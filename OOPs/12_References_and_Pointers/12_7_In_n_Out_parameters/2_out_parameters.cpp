#include <iostream>

void getCoordinates(int& x, int& y) {  // out parameters
    x = 10;
    y = 20;
}

int main() {
    int a, b;
    getCoordinates(a, b);  // a and b are modified

    std::cout << "a : " << a << "\nb : " << b << "\n";
}
