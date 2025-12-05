#include <iostream>
#include <array>
#include <utility>
#include <functional>

bool ascending_order(const int& a, const int& b)
{
    return (a > b);
}

bool descending_order(const int& a, const int& b)
{
    return (a < b);
}

bool even_front_ascending_order(const int& a, const int& b)
{
    if((a%2 == 0) && !(b%2 == 0))
        return false;

    if(!(a%2 == 0) && (b%2 == 0))
        return true;
    
    return ascending_order(a,b);
}

bool even_front_descending_order(const int& a, const int& b)
{
    if((a%2 == 0) && !(b%2 == 0))
        return false;

    if(!(a%2 == 0) && (b%2 == 0))
        return true;
    
    return descending_order(a,b);
}

bool odd_front_ascending_order(const int& a, const int& b)
{
    if((a%2 != 0) && !(b%2 != 0))
        return false;

    if(!(a%2 != 0) && (b%2 != 0))
        return true;
    
    return ascending_order(a,b);
}

bool odd_front_descending_order(const int& a, const int& b)
{
    if((a%2 != 0) && !(b%2 != 0))
        return false;

    if(!(a%2 != 0) && (b%2 != 0))
        return true;
    
    return descending_order(a,b);
}

void swap_it(int &a, int &b)
{
    int temp{};
    temp = std::move(a);
    a    = std::move(b);
    b    = std::move(temp);
}

//void selectionSort(int *arr, int size, bool (*comparison)(const int&, const int&) = ascending_order)
void selectionSort(int *arr, int size, std::function<bool(const int&, const int&)> comparison = ascending_order)
{
    if(!arr || !comparison)
        return;

    for(int i{}; i < size-1; i++)
    {
        int bestIndex{i};

        for(int j{i+1}; j < size; j++)
        {
            if( comparison(arr[bestIndex], arr[j]) )
                bestIndex = j;
        }
        
        swap_it(arr[i], arr[bestIndex]);
    }
}

void print(int *arr, int size)
{
    for(int i{}; i < size; i++)
        std::cout << arr[i] << " ";
    std::cout << "\n";
}

int main()
{
    int arr[]{3, 7, 9, 5, 6, 1, 8, 2, 4};
    //bool (*funcPtr)(const int&, const int&){};
    std::function<bool(const int&, const int&)> funcPtr{};
    
    funcPtr = &ascending_order;
    //funcPtr = &descending_order;
    //funcPtr = &even_front_ascending_order;
    //funcPtr = &even_front_descending_order;
    //funcPtr = &odd_front_ascending_order;
    //funcPtr = &odd_front_descending_order;

    //selectionSort(arr, std::size(arr));
    selectionSort(arr, std::size(arr), funcPtr);

    print(arr, std::size(arr));

    return 0;
}