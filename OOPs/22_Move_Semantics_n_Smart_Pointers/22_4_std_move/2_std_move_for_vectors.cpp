#include <iostream>
#include <utility>
#include <vector>

int main()
{
    std::vector<std::string> vs{};

    // We use std::string because it is movable (std::string_view is not)
    std::string str{"Rama"};

    std::cout << "Copying str\n";
    vs.push_back(str);              // calls l-value version of push_back, which copies str into the array element
    
    std::cout << "str : " << str << "\n";
    std::cout << "vs[0] : " << vs[0] << "\n";

    std::cout << "\nMoving str\n";
    vs.push_back(std::move(str));   // calls r-value version of push_back, which moves str into the array element

    std::cout << "str : " << str << "\n";           // The result of this is indeterminate
    std::cout << "vs[0] : " << vs[0] << "\tvs[1] : " << vs[1] << "\n";

    return 0;
}