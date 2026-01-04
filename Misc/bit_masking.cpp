#include <iostream>
#include <bitset>

bool is_power_of_two(int input)
{
    return ( (input > 0) && ( (input & (input-1)) == 0 ) );
}

int no_of_bit_sets(int input)
{
    int count{};

    while(input)
    {
        input &= (input -1);
        count++;
    }

    return count;
}

int main()
{
    int val{64};
    
    val = (val | (1 << 5));     // setting 5th bit in val; 64+32=96
    std::cout << val << "\n";
    std::cout << "is 5th bit set in val ? Answer : " << ( (val & (1<<5)) ? "Yes" : "No" ) << "\n\n";
    
    val = (val & (~(1 << 5)));  // unsetting 5th bit in val; 96-32=64  
    std::cout << val << "\n";
    std::cout << "is 5th bit set in val ? Answer : " << ( (val & (1<<5)) ? "Yes" : "No" ) << "\n\n";
    
    val = (val ^ (1 << 5));     // toggle 5th bit in val; 64+32=96 
    std::cout << val << "\n";
    std::cout << "is 5th bit set in val ? Answer : " << ( (val & (1<<5)) ? "Yes" : "No" ) << "\n\n";
    
    val = (val ^ (1 << 5));    // toggle 5th bit in val; 96-32=64
    std::cout << val << "\n";
    std::cout << "is 5th bit set in val ? Answer : " << ( (val & (1<<5)) ? "Yes" : "No" ) << "\n\n";    
    
    int input{};
    std::cout << "Enter number : ";
    std::cin >> input;

    if(is_power_of_two(input))
        std::cout << input << " is power of two.\n";
    else 
        std::cout << input << " is NOT power of two.\n";

    std::cout << input << " has " << no_of_bit_sets(input) << " set bits.\n";

    input &= (input-1); // clear the lowest set bit;
    std::cout << "Given input after clearing lowest set bit : " << input << "\n";

    return 0;
}