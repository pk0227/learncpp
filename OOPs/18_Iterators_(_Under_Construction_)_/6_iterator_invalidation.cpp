#include <iostream>
#include <vector>

int main()
{
    std::vector v{10, 20, 30, 40, 50};

    for(auto num : v)   // implicitly iterates over v
    {
        if(num%2 == 0)
            v.push_back(num+1);     // when this invalidates the iterators of v, undefined behavior will result
    }
    return 0;
}