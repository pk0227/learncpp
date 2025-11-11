#include <iostream>

//constexpr  std::string_view& getConstRef()
constexpr const std::string_view& getConstRef()     // function is constexpr, returns a const std::string_view&
{
    //static  std::string_view str{"local_string"};
    static constexpr std::string_view str{"local_string"};      //constexpr implcitly applies const

    return str;
}

int main()
{
    std::string str1{"main_string"};

    auto ref1{getConstRef()};  // ref1 has type std::string_view (const and reference dropped)
    constexpr auto ref1_constexpr{getConstRef()}; // ref1_constexpr has type constexpr std::string_view (const and reference dropped, constexpr applied, implicitly const)
    
    std::cout << "ref1 : " << ref1 << "\n";
    ref1 = str1;
    std::cout << "ref1 after modification : " << ref1 << "\n";
    std::cout << getConstRef() << "\n";  // static variable inside getConstRef remains unchanged
    
    std::cout << "--------------------------------------------------------------------------------\n";

    auto& ref2{getConstRef()};  // ref2 has type std::string_view (const and reference reapplied)
    constexpr const auto& ref2_constexpr{getConstRef()}; // ref2_constexpr has type constexpr const std::string_view& (reference reapplied, low-level const not dropped, constexpr applied)
    
    std::cout << "ref2 : " << ref2 << "\n";
    //ref2 = str1;   // modifying ref2 modifies the static variable inside getConstRef 
                   // unless return type of getConstRef is changed to constexpr const std::string_view& and constexpr used to static string_view str inside getConstRef.
    std::cout << "ref2 after modification : " << ref2 << "\n";
    std::cout << getConstRef() << "\n";  // static variable inside getConstRef is modified

    return 0;
}