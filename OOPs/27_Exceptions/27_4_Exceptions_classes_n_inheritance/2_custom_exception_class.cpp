#include <iostream>
#include <string>
#include <utility>
#include <format>

class ArrayException
{
    std::string m_error{};
public:
    ArrayException(std::string err="") : m_error{std::move(err)}
    { }

    const std::string& getError() const { return m_error; }
};


template <typename T, int size>
class GenericArray
{
    T m_array[size];
public:
    int getSize() const { return size; }
    T& operator[](std::size_t index)
    {
        if(index < 0 || index >= static_cast<std::size_t>(size))
            throw ArrayException{"Invalid Index"};
        return m_array[index];
    }
};

int main()
{
    GenericArray<int, 10> gai{};
    int index{10};
    try
    {
        std::cout << std::format("gai[{}] : {}", index, gai[index]) << "\n";
    }
    catch(const ArrayException& e)
    {
        std::cerr << "Error : " << e.getError() << "\n";
    }

    return 0;
}