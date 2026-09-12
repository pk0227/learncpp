// Demonstrates C++20 constinit keyword
// Highlights compile-time initialization of static/global variables while keeping them mutable at runtime

#include <iostream>

constexpr int getInitialValue(int factor)
{
    return factor * 10;
}

// 1. constinit on global variable:
// Guarantees compile-time initialization. Prevents Static Initialization Order Fiasco (SIOF).
constinit int g_requestCount = getInitialValue(5); // Initialized at compile-time to 50

// Unlike constexpr, constinit variables are NOT const! They can be modified at runtime:
void logRequest()
{
    ++g_requestCount; // OK: mutable
}

void demonstrateStaticLocal()
{
    // 2. constinit on static local variable:
    constinit static int callCount = getInitialValue(1);
    ++callCount;
    std::cout << "Call count: " << callCount << '\n';
}

int main()
{
    std::cout << "Initial g_requestCount: " << g_requestCount << '\n';
    logRequest();
    logRequest();
    std::cout << "Updated g_requestCount: " << g_requestCount << '\n';

    demonstrateStaticLocal();
    demonstrateStaticLocal();

    return 0;
}
