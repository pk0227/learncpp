#include <iostream>
#include <vector>
#include <string>
#include <string_view>

class Foo
{
    std::string m_str{};
    int m_i{};
public:
    Foo(std::string_view str, int i) : m_str{str}, m_i{i}
    { }

    explicit Foo(int x) : m_str{}, m_i{x}
    { }
};

void testExplicit(Foo obj)
{
    std::cout << "testExplicit called\n";
}

int main()
{
    std::vector<Foo> stack{};

    Foo f{"abc", 12};
    stack.push_back(f);
    stack.emplace_back(f);

    stack.push_back({"xyz", 23});
    //stack.push_back("mno", 34);
    stack.emplace_back("mno", 45);
    
    //stack.push_back({2});
    stack.emplace_back(2);
    
    std::cout << "-----\n";
    
    return 0;
}