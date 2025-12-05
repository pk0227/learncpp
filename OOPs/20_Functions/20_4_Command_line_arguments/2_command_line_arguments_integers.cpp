#include <iostream>
#include <sstream>

int main(int argc, char** argv)
{
    std::stringstream convert{};
    int intArg{};

    for(int i{}; i < argc; i++)
    {
        convert.clear();
        convert.str(argv[i]);
        convert >> intArg;
        std::cout << intArg << " ";
    }
    std::cout << "\n";

    return 0;
}