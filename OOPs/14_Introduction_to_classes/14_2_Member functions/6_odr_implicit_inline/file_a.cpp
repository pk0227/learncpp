// Translation unit A. Includes counter.h (so it sees the full definitions of
// Counter::increment() and Counter::get()).
#include "counter.h"

// A function that uses Counter in this TU.
int useCounterInA()
{
    Counter c;
    c.increment();
    c.increment();
    return c.get(); // 2
}
