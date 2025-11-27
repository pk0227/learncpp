#include <iostream>
#include <functional>
#include <vector>
#include <string>

int main()
{
    //std::vector<std::reference_wrapper<const std::string>> names{std::string{"Rama"}};  // conceptually it should work, const reference should be able to hold rvalue.
                                                                                        // but, C++ doesn't allow a reference_wrapper to an rvalue temporary.
                                                                                        // It must still bind to an existing object with a stable lifetime.

    std::string s1{"Rama"};
    std::string s2{"Laxmana"};
    std::string s3{"Hanuman"};

    std::vector<std::reference_wrapper<std::string>> names{s1, s2};

    names.emplace_back(s3);     // push_back inserts an existing object (copy/move), while emplace_back constructs the object directly in place and avoids temporary creation. emplace_back constructs the new element only after reallocation, ensuring optimal placement.

    for(auto name : names)
    {   
        name.get() += " ----- ";
    }

    std::cout << s1 << "\n";
    return 0;
}