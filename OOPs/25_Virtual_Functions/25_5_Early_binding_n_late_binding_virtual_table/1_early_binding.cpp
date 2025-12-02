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

    int result {};
    switch (op)
    {
        // call the target function directly using early binding
        case 0: result = add(x, y); break;
        case 1: result = subtract(x, y); break;
        case 2: result = multiply(x, y); break;
        default:
            std::cout << "Invalid operator\n";
            return 1;
    }

    std::cout << "The answer is: " << result << '\n';

    return 0;
}