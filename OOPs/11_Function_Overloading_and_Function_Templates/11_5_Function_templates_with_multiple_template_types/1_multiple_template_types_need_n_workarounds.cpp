#include <iostream>

template <typename T>
T max(T x , T y)
{
    return (x < y) ? y : x;
}

int main()
{
    //std::cout << max(1, 2.3) << '\n';   // no matching function for call to 'max(int, double)'. 
    /*
        You might wonder why the compiler didn’t generate function max<double>(double, double) and then use numeric conversion to type convert the int argument to a double. 
        The answer is simple: type conversion is done only when resolving function overloads, not when performing template argument deduction.
    */

   // convert our int to a double so we can call max(double, double)
   std::cout << max(static_cast<double>(1), 2.3) << " : " << typeid(max(static_cast<double>(1), 2.3)).name() << '\n';               
   
   // we've explicitly specified type double, so the compiler won't use template argument deduction
   std::cout << max<double>(1, 2.3) << " : " << typeid(max<double>(1, 2.3)).name() << '\n';                

   return 0;
}