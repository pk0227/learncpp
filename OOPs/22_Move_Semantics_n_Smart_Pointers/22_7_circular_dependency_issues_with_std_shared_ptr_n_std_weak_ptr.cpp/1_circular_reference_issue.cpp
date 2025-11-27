#include <iostream>
#include <memory>
#include <utility>

class Person
{
    std::string m_name{};
    std::shared_ptr<Person> m_partner{};
public:
    Person(const std::string name) : m_name{std::move(name)}
    {
        std::cout << "Constructed...!\n";
    }

    ~Person()
    {
        std::cout << "Destructed...!\n";
    }

    friend bool partnerUp(const std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2)
    {
        if(!p1 || !p2)
            return false;

        p1->m_partner = p2;
        p2->m_partner = p1;

        std::cout << p1->m_name << " is now partnered with " << p2->m_name << "\n";
        return true;
    }
};

int main()
{
    auto rama{std::make_shared<Person>("Rama")};
    auto seetha{std::make_shared<Person>("Seetha")};

    partnerUp(rama, seetha);
    
    return 0;
}