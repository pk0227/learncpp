#include <iostream>
#include <memory>
#include <utility>

class Person
{
    std::string m_name{};
    std::weak_ptr<Person> m_partner{};
public:
    Person(const std::string name) : m_name{std::move(name)}
    {
        std::cout << m_name << " - Constructed...!\n";
    }

    ~Person()
    {
        std::cout << m_name << " - Destructed...!\n";
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
    const std::string& getName() const { return m_name; }
    std::shared_ptr<Person> getPartner() const
    {
        return m_partner.lock();
    }
};

int main()
{
    auto male{std::make_shared<Person>("Male")};
    auto female{std::make_shared<Person>("Female")};

    partnerUp(male, female);

    auto partner{male->getPartner()};

    std::cout << male->getName() << "'s partner is : " << partner->getName() << "\n";    
    return 0;
}