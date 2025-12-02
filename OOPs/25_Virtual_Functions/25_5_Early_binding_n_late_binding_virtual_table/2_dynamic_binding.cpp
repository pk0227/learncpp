#include <iostream>

int add(int x, int y)
{
    return x + y;
}

int subtract(int x, int y)
{
    return x - y;
}

int multiply(int x, int y)
{
    return x * y;
}

int main()
{
    int x{};
    std::cout << "Enter a number: ";
    std::cin >> x;

    int y{};
    std::cout << "Enter another number: ";
    std::cin >> y;

    int op{};
    std::cout << "Enter an operation (0=add, 1=subtract, 2=multiply): ";
    std::cin >> op;

    using FcnPtr = int (*)(int, int); // alias ugly function pointer type
    FcnPtr fcn { nullptr }; // create a function pointer object, set to nullptr initially

    // Set fcn to point to the function the user chose
    switch (op)
    {
        case 0: fcn = add; break;
        case 1: fcn = subtract; break;
        case 2: fcn = multiply; break;
        default:
            std::cout << "Invalid operator\n";
            return 1;
    }

    // Call the function that fcn is pointing to with x and y as parameters
    std::cout << "The answer is: " << fcn(x, y) << '\n';

    return 0;
}