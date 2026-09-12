#include <iostream>

class UD
{
    public:
        operator int(){ return 0; }       // user-defined conversion from UD to int as its return type is int.
                                            // exact match for int parameter function is found.
            
        //operator char(){ return 0; }      // user-defined conversion from UD to char as its return type is char.
                                            // char is promoted to int for int parameter function.
                              
        //operator double (){ return 0; }   // user-defined conversion from UD to double as its return type is double.
                                            // exact match for double parameter function is found.

        // operator long int(){ return 0; } // Note: if only operator long int() is enabled, long can convert to
                                            // both int and double, causing an ambiguous match compile error at func(x).

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