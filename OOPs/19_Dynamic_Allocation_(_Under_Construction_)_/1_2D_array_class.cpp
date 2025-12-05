#include <iostream>
#include <iomanip>

template <typename T>
class TwoDArray
{
    //T** m_array{};        // Declaring m_array before m_rows & m_cols causes exception at line 24 during Dynamic Memory Allocation
                            // if we use m_rows in new operator. Because members in members initialier list will get initialized in the
                            // order they have been defined. 
    std::size_t m_rows{};
    std::size_t m_cols{};
    T** m_array{};

    class RowClass
    {
        T* m_RowArray{};
    public:
        RowClass(T* row_ptr) : m_RowArray{row_ptr}
        { }
        T& operator[](std::size_t col)
        {
            return m_RowArray[col];
        }
    };

public:
    TwoDArray(std::size_t rows, std::size_t cols) : m_rows{rows}, m_cols{cols}, m_array{new T*[m_rows]}
    { 
        for(std::size_t i{}; i < m_rows; i++)
            m_array[i] = new T[m_cols];
    }

    ~TwoDArray()
    { 
        for(std::size_t i{}; i < m_rows; i++)
            delete[] m_array[i];
        delete[] m_array;
    }

    RowClass operator[](std::size_t row)
    {
        return RowClass{m_array[row]};
    }

    int getRowsCount() const { return m_rows; }
    int getColsCount() const { return m_cols; }
};

void loadTwoDArray(auto& ref)
{
    for(int i{}; i < ref.getRowsCount(); i++)
    {
        for(int j{}; j < ref.getColsCount(); j++)
        {
            ref[i][j] = i+j+10;
            //ref[i][j] = 150.00/(i+j+10);
        }
    }
}

void printTwoDArray(auto& ref)
{
    auto oldFlags{std::cout.flags()};
    std::cout << std::left;

    for(int i{}; i < ref.getRowsCount(); i++)
    {
        for(int j{}; j < ref.getColsCount(); j++)
            std::cout << std::setw(10) << ref[i][j] << " ";
        std::cout << "\n";
    }

    std::cout.flags(oldFlags);
}

int main()
{
    TwoDArray<int> tda1(10,8);
    //TwoDArray<double> tda1(10,8);
    loadTwoDArray(tda1);
    printTwoDArray(tda1);

    return 0;
}