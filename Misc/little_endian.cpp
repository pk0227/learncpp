#include <iostream>
#include <bitset>

int main()
{
    int endian {0x12345678};
    unsigned char *ptr {reinterpret_cast<unsigned char *>(&endian)};
    
    std::cout << std::hex << endian << "\n";
    std::cout << static_cast<int>(ptr[0]) << "\n";
    
    if(ptr[0] == 0x78)
    {
        std::cout << "Little Endian\n";
    }
    else
        std::cout << "Big Endian\n";
    
    return 0;
}