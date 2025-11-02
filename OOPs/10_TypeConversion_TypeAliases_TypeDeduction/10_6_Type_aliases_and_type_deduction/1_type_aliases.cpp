#include <iostream>
#include <string> // for std::string
#include <vector> // for std::vector
#include <utility> // for std::pair

typedef int (*FcnType)(double, char); // FcnType hard to find
using FcnType = int(*)(double, char); // FcnType easier to find

using VectPairSI = std::vector<std::pair<std::string, int>>; // make VectPairSI an alias for this crazy type

bool hasDuplicates(VectPairSI pairlist) // use VectPairSI in a function parameter
{
    // some code here
    return false;
}

int main()
{
     VectPairSI pairlist; // instantiate a VectPairSI variable

     std::cout << "Size of pairlist: " << pairlist.size() << '\n';
     return 0;
}