#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>

char* make_big_str(const char* str, std::ptrdiff_t times)
{
    char *big_str{};
    for(auto i{0Z}; i<times; i++)
    {
        std::size_t big_str_size{};
        if(big_str)
            big_str_size = std::strlen(big_str);
        else 
            big_str_size = 0;

        char *new_str = new char[big_str_size+std::strlen(str)+1]{};
        if(big_str)
            std::copy_n(big_str, big_str_size, new_str);

        std::copy_n(str, std::strlen(str), new_str+big_str_size);

        delete[] big_str;
        big_str = new_str;
    }
    return big_str;
}

int main()
{
    const char* str { "abcdefghijklmnopqrstuvwxyz" };

    char *big_str { make_big_str(str, 2581111) };

    std::cout << big_str << "\n";

    return 0;
}
