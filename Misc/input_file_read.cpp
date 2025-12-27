#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cstring>
#include <charconv>

int main()
{
    std::ifstream ifs{"abc.txt"};
    std::string str{};
    int total_sum{};
    
    if(!ifs)
    {
        std::cerr << "Failed to open...!\n";
        return -1;
    }

    while(std::getline(ifs, str))
    {
        std::cout << str << "\n";
        //std::cout << "length() : " << str.length() << "\n";
        //std::cout << "size() : " << str.size() << "\n";
        
        auto pos{std::string::npos};
        int num{};
        pos = str.find("-- ");
        
        if(pos == std::string::npos)
            continue;
    
        std::cout << "number : " << str.substr(pos+3) << "\n";
        auto [ptr, ec] { std::from_chars(str.c_str()+pos+3, str.c_str()+str.length()-pos-3, num)};
        
        if (ec == std::errc{}) {
            std::cout << "Parsed value: " << num << "\n";
        } else {
            std::cout << "Conversion failed! ec = " << static_cast<int>(ec) << "\n";
        }
        
        total_sum += num;
        
        std::cout << "=============================\n";
    }
    
    ifs.close();
    
    std::cout << "\n\nTotal sum : " << total_sum << "\n";

    return 0;
}