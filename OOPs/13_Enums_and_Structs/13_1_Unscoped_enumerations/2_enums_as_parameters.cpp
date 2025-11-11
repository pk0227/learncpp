#include <iostream>

enum SortOrder
{
    alphabetical,
    alphabeticalReverse,
    numerical,
};

void sortData(SortOrder order)
{
    switch (order)
    {
        case alphabetical:
            std::cout << "sort data in forwards alphabetical order\n";
            break;
        case alphabeticalReverse:
            std::cout << "sort data in forwards alphabeticalReverse order\n";
            break;
        case numerical:
            std::cout << "sort data in forwards numerical order\n";
            break;
    }
}

int main()
{
    SortOrder order{alphabeticalReverse};
    //sortData(1);        // error: invalid conversion from 'int' to 'SortOrder'
    sortData(order);
    sortData(alphabetical);
    return 0;
}