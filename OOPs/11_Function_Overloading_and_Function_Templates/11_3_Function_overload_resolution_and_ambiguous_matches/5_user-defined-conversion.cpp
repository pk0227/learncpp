#include <iostream>

class UD
{
    public:
        //operator int(){ return 0; }       // user-defined conversion from UD to int as it return type is int.
                                            // exact match for int parameter function is found. So, no conversion.
            
        //operator char(){ return 0; }      // user-defined conversion from UD to char as it return type is char.
                                            // char is promoted to int for int parameter function.
                              
        //operator double (){ return 0; }     // user-defined conversion from UD to double as it return type is double.
                                            // exact match for double parameter function is found. So, no conversion.

        operator long int(){ return 0; }  // user-defined conversion from UD to long as it return type is long.
                                            // long can be converted to int and double. So, ambiguity occurs between int and double parameter functions.

};

void func(int i)
{
    std::cout << "int func : " << i << std::endl;
}

void func(double d)
{
    std::cout << "double func : " << d << std::endl;
}

int main()
{
    UD x;
    func(x);    // x is converted to type int using the user-defined conversion from UD to int

    return 0;
}