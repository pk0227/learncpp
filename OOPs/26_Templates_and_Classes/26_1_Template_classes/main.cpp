#include <iostream>
#include "templates.cpp"

void printElements(const GenericArray<int>& ref)
{
    //for(const auto &i : gi) // 'begin' was not declared in this scope
    for(int i{}; i < ref.getLength(); i++)
    {
        std::cout << ref[i] << " ";
    }
    std::puts("");
}

void loadElements(GenericArray<int>& ref)
{
   for(int i{}; i < ref.getLength(); i++)
    {
        ref[i] = ((i+1)*10);
    }
}

int main()
{
    GenericArray<int> gi{10, 20, 54, 87, 96};
    std::cout << "Length : " << gi.getLength() << "\n";
    printElements(gi);
    std::cout << "---------------------------------------------------------\n";

    gi.resize(3);
    std::cout << "Length : " << gi.getLength() << "\n";
    printElements(gi);
    std::cout << "---------------------------------------------------------\n";
    
    GenericArray<int> gi1{};
    std::cout << "Length : " << gi1.getLength() << "\n";
    std::cout << "---------------------------------------------------------\n";

    GenericArray<int> gi2(6);
    loadElements(gi2);
    std::cout << "Length : " << gi2.getLength() << "\n";
    printElements(gi2);
    std::cout << "---------------------------------------------------------\n";

    gi2.reallocate(10);
    loadElements(gi2);
    std::cout << "Length : " << gi2.getLength() << "\n";
    printElements(gi2);
    std::cout << "---------------------------------------------------------\n";
 
    GenericArray<int> gi3{gi2};
    std::cout << "Length : " << gi3.getLength() << "\n";
    printElements(gi3);
    std::cout << "---------------------------------------------------------\n";
 
    gi3 = gi;
    std::cout << "Length : " << gi3.getLength() << "\n";
    printElements(gi3);
    std::cout << "---------------------------------------------------------\n";

    gi3 = {21, 31, 41, 51};
    std::cout << "Length : " << gi3.getLength() << "\n";
    printElements(gi3);
    std::cout << "---------------------------------------------------------\n";

    if(gi3.insert(2, 212))
    {
        std::cout << "Length : " << gi3.getLength() << "\n";
        printElements(gi3);
        std::cout << "---------------------------------------------------------\n";
    }
    else
        std::cout << "Failed to Insert\n-----------------------------------------\n";

    if(gi3.insert(-5,515))
    {
        std::cout << "Length : " << gi3.getLength() << "\n";
        printElements(gi3);
        std::cout << "---------------------------------------------------------\n";
    }
    else
        std::cout << "Failed to Insert\n-----------------------------------------\n";

    if(gi3.insert(5,515))
    {
        std::cout << "Length : " << gi3.getLength() << "\n";
        printElements(gi3);
        std::cout << "---------------------------------------------------------\n";
    }
    else
        std::cout << "Failed to Insert\n-----------------------------------------\n";

    
    if(gi3.remove(4))
    {
        std::cout << "Length : " << gi3.getLength() << "\n";
        printElements(gi3);
        std::cout << "---------------------------------------------------------\n";
    }
    else
        std::cout << "Failed to Remove\n-----------------------------------------\n";

    if(gi3.remove(-5))
    {
        std::cout << "Length : " << gi3.getLength() << "\n";
        printElements(gi3);
        std::cout << "---------------------------------------------------------\n";
    }
    else
        std::cout << "Failed to Remove\n-----------------------------------------\n";

    if(gi3.remove(5))
    {
        std::cout << "Length : " << gi3.getLength() << "\n";
        printElements(gi3);
        std::cout << "---------------------------------------------------------\n";
    }
    else
        std::cout << "Failed to Remove\n-----------------------------------------\n";

    return 0;
}