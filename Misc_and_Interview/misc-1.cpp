#include <iostream>
#include <cstring>
#include <string>

const char* getBigStr(const char* str, std::size_t times)
{
    /*
    std::size_t str_length = std::strlen(str);    
    char* curr_str{nullptr};
    std:size_t curr_length{}; 
    */
    std::string input{str}, output{};
    
    for(std::size_t i{}; i < times; i++)
    {
        /*
        if(curr_str)
            curr_length = std::strlen(curr_str);

        char* temp = (char *)calloc(curr_length + str_length +1, 1);
        //std::strncpy(temp, curr_str, curr_length);
        //std::strncpy(temp+curr_length, str, str_length);
        std::copy_n(curr_str, curr_length, temp);
        std::copy_n(str, str_length, temp+curr_length);
        temp[curr_length+str_length+1] = 0;
        
        if(curr_str)
            free(curr_str);
        
        curr_str = temp;
        */
        output += input;
    }    
    
    //return curr_str;
    char* temp = (char *)calloc(output.length()+1, 1);
    //std::strncpy(temp, output.c_str(), output.length());
    std::copy_n(output.c_str(), output.length(), temp);
    temp[output.length()] = 0;
    
    return temp;
}

int main()
{
    const char* str{"abcdefghijklmnopqrstuvwxyz"};
    const char* final_str{getBigStr(str, 2581111)};
    
    std::cout << std::strlen(final_str) << "\n";

    free((void*)final_str);
    return 0;
}