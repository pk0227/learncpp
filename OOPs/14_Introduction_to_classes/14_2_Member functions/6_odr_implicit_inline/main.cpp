// Translation unit B (main). ALSO includes counter.h -- so the SAME member
// function definitions (increment/get) now exist in both this TU and file_a.cpp.
//
// If in-class member functions were NOT implicitly inline, linking file_a.cpp
// and main.cpp together would fail with a "multiple definition" error for
// Counter::increment() and Counter::get(). It links cleanly precisely BECAUSE
// those in-class definitions are implicitly inline (ODR-exempt across TUs).
#include "counter.h"
#include <iostream>

int useCounterInA(); // defined in file_a.cpp

int main()
{
    Counter c;
    c.increment();
    std::cout << label() << " in main after 1 increment: " << c.get() << '\n'; // 1
    std::cout << label() << " from file_a.cpp:           " << useCounterInA() << '\n'; // 2

    std::cout << "Linked two translation units that both define "
                 "Counter's member functions -- no ODR violation,\n"
                 "because in-class member functions are implicitly inline.\n";
    return 0;
}
