#include <iostream>
#include <vector>

int main()
{
    std::vector v{10, 20, 30, 40, 50};
    auto it{v.begin()};
    ++it;                           // move to second element
    std::cout << *it << "\n";

    v.erase(it);                    // erase the element currently being iterated over
                                    // erase() invalidates iterators to the erased element (and subsequent elements)
	                                // so iterator "it" is now invalidated
                                    
    //it = v.erase(it);             // erase the element currently being iterated over, set `it` to next element

    ++it;                           // undefined behavior
    std::cout << *it << "\n";       // undefined behavior

    return 0;
}