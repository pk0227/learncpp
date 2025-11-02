#include <iostream>

void addAndPrint(auto x, auto y)
{
    std::cout << x + y << '\n';
}

int main()
{
    addAndPrint(2, 3); // case 1: call addAndPrint with int parameters
    addAndPrint(4.5, 6.7); // case 2: call addAndPrint with double parameters

    return 0;
}

// prior to C++20, the above program won’t compile.
// In C++20, the auto keyword was extended so that the above program will compile and function correctly -- however, auto is not invoking type deduction in this case.
// Rather, it is triggering a different feature called function templates that was designed to actually handle such cases.