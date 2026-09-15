#include <iostream>

class outer;         // okay: can forward declare non-nested type
// COMPILE ERROR: can't forward declare nested type prior to outer class definition (outer is incomplete):
// class outer::inner1;

class outer
{
public:
    class inner1{}; // note: nested type declared here
};

class outer::inner1; // okay (but redundant) since nested type has already been declared as part of outer class definition

int main()
{
    return 0;
}