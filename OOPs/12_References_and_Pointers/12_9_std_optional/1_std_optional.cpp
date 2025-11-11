#include <iostream>
#include <optional>


struct Employee
{
    std::string name{};
    int id;
};

std::optional<int> doIntDivision(int num, int den)
{
    if(den == 0)
        return {};
    
    return num/den;
}

//void print(std::optional<std::reference_wrapper<int>> e=std::nullopt)
void print(const std::optional<int> &e=std::nullopt)
{
    if(e)
        std::cout << "Result : " << *e << "\n";
    else
        std::cout << "Result : doIntDivision failed\n";
}

//void print(std::optional<std::reference_wrapper<Employee>> e=std::nullopt)    // std::optional mimics its reference with help of reference_wrapper
void print(const std::optional<Employee> &e=std::nullopt)
{
    if(e)
    {
    //    std::cout << "Name : " << e->get().name << "\n" << "id : " << e->get().id << "\n";
        std::cout << "Name : " << (*e).name << "\n" << "id : " << (*e).id << "\n";
    }
    else
        std::cout << "Print failed\n";
}

int main()
{
    std::optional<int> res1{doIntDivision(20, 4)};

    print(res1);

    Employee emp{"Rama", 31};

    print(emp);

    return 0;
}