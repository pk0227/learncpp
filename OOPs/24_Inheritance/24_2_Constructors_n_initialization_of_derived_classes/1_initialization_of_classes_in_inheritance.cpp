#include <iostream>
#include <utility>

class Person
{
    std::string m_name{};
    int m_age{};

public:
    Person(std::string name="", int age=0) : m_name{std::move(name)}, m_age{std::move(age)}
    { 
        std::cout << "Person - Constructed\n"; 
    }

    ~Person()
    {
        std::cout << "Person - Destructed\n";        
    }

    const std::string& getName() const { return m_name; }
    int getAge() const { return m_age; }

};

class CricketPlayer : public Person
{
    double m_battingAverage{};
    int m_centuries{};

public:
    CricketPlayer(std::string name="", int age=0, double avgBatting=0.0, int centuries=0) 
                    : Person{std::move(name), std::move(age)}
                    , m_battingAverage{avgBatting}
                    , m_centuries{centuries}  
    { 
        std::cout << "CricketPlayer - Constructed\n"; 
    }

    ~CricketPlayer()
    {
        std::cout << "CricketPlayer - Destructed\n";        
    }

    const double& getBattingAverage() const { return m_battingAverage; }
    int getTotalcenturies() const { return m_centuries; }

};

int main()
{
    CricketPlayer player1{"Virat Kohli", 37, 57.7, 82};
    std::cout << player1.getName() << "\n";
    std::cout << player1.getAge() << "\n";
    std::cout << player1.getBattingAverage() << "\n";
    std::cout << player1.getTotalcenturies() << "\n";
    
    return 0;
}