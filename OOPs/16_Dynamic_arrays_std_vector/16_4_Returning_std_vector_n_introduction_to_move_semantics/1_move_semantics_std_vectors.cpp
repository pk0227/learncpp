#include <iostream>
#include <vector>

std::vector<int> getVector()        // return by value
{
    // We're intentionally using a named object here so mandatory copy elision doesn't apply
    std::vector arr1{1, 2, 3, 4, 5};       // copies { 1, 2, 3, 4, 5 } into arr1
    return arr1;
}

int main()
{
    std::vector arr2{getVector()};      // the return value of generate() dies at the end of the expression
    
    // There is no way to use the return value of generate() here
    arr2[0] = 7;

    std::cout << arr2[0] << "\n";

    return 0;
}