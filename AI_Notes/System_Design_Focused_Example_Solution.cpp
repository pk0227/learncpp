#include <iostream>
#include <cstring>
#include <cstddef>

/*
 * Dynamically growing string buffer
 * Similar in spirit to std::string internals
 */
struct dyn_str
{
    char* data{};            // allocated buffer
    std::size_t size{};      // number of valid characters (excluding '\0')
    std::size_t capacity{};  // allocated capacity
};

/*
 * Append a C-string to dyn_str
 * Grows capacity geometrically when needed
 */
void append(dyn_str& s, const char* str)
{
    const std::size_t len = std::strlen(str);

    // Ensure enough capacity (including null terminator)
    if (s.size + len + 1 > s.capacity)
    {
        std::size_t new_cap = s.capacity ? s.capacity * 2 : 64;

        while (new_cap < s.size + len + 1)
            new_cap *= 2;

        char* new_buf = new char[new_cap];

        if (s.data)
            std::memcpy(new_buf, s.data, s.size);

        delete[] s.data;
        s.data = new_buf;
        s.capacity = new_cap;
    }

    // Append new data
    std::memcpy(s.data + s.size, str, len);
    s.size += len;
    s.data[s.size] = '\0';   // maintain null termination
}

/*
 * Cleanup helper
 */
void destroy(dyn_str& s)
{
    delete[] s.data;
    s.data = nullptr;
    s.size = 0;
    s.capacity = 0;
}

int main()
{
    dyn_str s{};

    const char* chunk = "abcdefghijklmnopqrstuvwxyz";

    // Simulate unknown / real-time iterations
    for (std::size_t i = 0; i < 2581111; ++i)
    {
        append(s, chunk);
    }

    std::cout << "Final size  : " << s.size << "\n";
    std::cout << "Capacity    : " << s.capacity << "\n";
    std::cout << "First 100 chars:\n";
    std::cout.write(s.data, 100);
    std::cout << "\n";

    destroy(s);
    return 0;
}
