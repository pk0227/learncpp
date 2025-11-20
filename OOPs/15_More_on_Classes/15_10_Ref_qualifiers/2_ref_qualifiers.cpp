#include <iostream>
#include <string>
#include <string_view>

class Employee
{
private:
	std::string m_name{};

public:
	Employee(std::string_view name): m_name { name } {}

    // First, for a given function, non-ref-qualified overloads and ref-qualified overloads cannot coexist. Use one or the other.
	//const std::string& getName() const  { return m_name; } 

    // Second, similar to how a const lvalue reference can bind to an rvalue, if only a const lvalue-qualified function exists, it will accept either lvalue or rvalue implicit objects.
	const std::string& getName() const &  
    {
        std::cout << "lvalue object : ";
        return m_name; 
    } //  & qualifier overloads function to match only lvalue implicit objects

    // Third, either qualified overload can be explicitly deleted (using = delete), which prevents calls to that function. For example, deleting the rvalue-qualified version prevents use of the function with rvalue implicit objects.
    std::string  getName() const && = delete;
/*
    std::string  getName() const && 
    {
        std::cout << "rvalue object : "; 
        return m_name; 
    } // && qualifier overloads function to match only rvalue implicit objects
*/
};

// createEmployee() returns an Employee by value (which means the returned value is an rvalue)
Employee createEmployee(std::string_view name)
{
	Employee e { name };
	return e;
}

int main()
{
	Employee joe { "Joe" };
	std::cout << joe.getName() << '\n'; // Joe is an lvalue, so this calls std::string& getName() & (returns a reference)

	std::cout << createEmployee("Frank").getName() << '\n'; // Frank is an rvalue, so this calls std::string getName() && (makes a copy)

	return 0;
}