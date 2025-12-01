#include <iostream>
#include <string_view>

class A
{
public:
    virtual std::string_view getName() const { return "A"; }
};

class B : public A
{
public:
    virtual std::string_view getName() const { return "B"; }
};

class C : public B
{
public:
    std::string_view getName() const { return "C"; }    // if virtual keyword is not explicitly mentioned,
                                                        // If a function is virtual, all matching overrides in derived classes are implicitly virtual.
                                                        
};

class D : public C
{
public:
    virtual std::string_view getName() const { return "D"; } 
};

int main()
{
    C c{};
    A& rBase{c};
    std::cout << "rBase is a " << rBase.getName() << "\n";

    return 0;
}