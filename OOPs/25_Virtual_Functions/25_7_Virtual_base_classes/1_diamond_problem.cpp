#include <iostream>

class PowerDevice
{
public:
    PowerDevice(int power)
    {
        std::cout << "PowerDevice : " << power << "\n";
    }
};

//class Scanner : public PowerDevice
class Scanner : virtual public PowerDevice
{
public:
    Scanner(int scanner, int power) : PowerDevice{power}
    {
        std::cout << "Scanner : " << scanner << "\n";
    }
};

//class Printer : public PowerDevice
class Printer : virtual public PowerDevice
{
public:
    Printer(int printer, int power) : PowerDevice{power}
    {
        std::cout << "Printer : " << printer << "\n";
    }
};

class Copier : public Scanner, public Printer
{
public:
    Copier(int copier, int scanner, int printer, int power) 
            : Scanner{scanner, power}
            , Printer{printer, power}
            , PowerDevice{power}
    {
        std::cout << "Copier : " << copier << "\n";
    } 
};

int main()
{
    Copier c{1, 2, 3, 4};

    return 0;
}