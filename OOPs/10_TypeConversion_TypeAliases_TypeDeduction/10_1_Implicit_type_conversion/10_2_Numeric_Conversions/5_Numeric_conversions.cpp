#include <iostream>

int main()
{
    // Converting an integral type to any other integral type (excluding integral promotions):
    short s = 3;        // convert int to short
    long l = 3;         // convert int to long
    char ch = s;        // convert short to char
    unsigned int u = 3; // convert int to unsigned int

    //Converting a floating point type to any other floating point type (excluding floating point promotions):
    float f = 3.0;        // convert double to float
    long double ld = 3.0; // convert double to long double

    // Converting a floating point type to any integral type:
    int i = 3.5;                                                                  // convert double to int
    
    //Converting an integral type to any floating point type : 
    double d = 3;        // convert int to double
    
    //Converting an integral type or a floating point type to a bool : 
    bool b1 = 3; // convert int to bool
    bool b2 = 3.0;                                                                // convert double to bool

    return 0;
}