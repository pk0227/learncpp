#include <iostream>

int main(int argc, char** argv)
{
    for(int i{}; i < argc; i++)
        std::cout << argv[i] << "\n";
    
    return 0;
}