#include <iostream>
#include <vector>

int fibonacci(int count)
{
    static std::vector<int> results{0, 1};

    if(count < std::size(results))
        return results[count];
    
    results.push_back(fibonacci(count-1)+fibonacci(count-2));
    return results[count]; 
}

int main()
{
    for(int i{}; i < 15; i++)
        std::cout << fibonacci(i) << " ";
    std::cout << "\n";
    
    return 0;
}