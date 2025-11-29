#include <iostream>
#include <string>

class Car
{
    std::string m_manufacturer{};
    int m_ID{};

public:
    Car(std::string manufacturer="", int id=0) : m_manufacturer{std::move(manufacturer)}, m_ID{std::move(id)}
    { }

    const std::string& getName() const { return m_manufacturer; }
    int getID() const { return m_ID; }
};

namespace CarLot
{
    Car carlot[4]{Car{"Suzuki", 1001}, Car{"Hyundai", 1002}, Car{"Tata", 1003}, Car{"Mahindra", 1004}};

    Car* getCar(int id)
    {
        for(auto& car : carlot)
        {
            if(car.getID() == id)
                return &car;
        }

        return nullptr;
    }
};

class Driver
{
    std::string m_name{};
    int m_car_id{};

public:
    Driver(std::string name="", int car_id=0) : m_name{std::move(name)}, m_car_id{std::move(car_id)}
    { }

    const std::string& getName() const { return m_name; }
    int getID() const { return m_car_id; }
};


int main()
{
    Driver d{"Bob", 1002};

    Car* car{CarLot::getCar(d.getID())};

    if(car)
        std::cout << d.getName() << " is driving a " << car->getName() << " car\n";
    else
        std::cout << d.getName() << " couldn't find his car\n";

    return 0;
}