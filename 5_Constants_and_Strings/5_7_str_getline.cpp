#include <iostream>
#include <string> // For std::string and std::getline

int main()
{
    std::cout << "Enter your full name : ";
    std::string name{};
    //std::getline reads the line including \n. But,it discards the \n and stores the line into given variable.
    //std::getline(std::cin >> std::ws, name);    
    std::getline(std::cin, name);

    std::cout << "Enter your age : ";
    int age{};
    std::cin >> age;                            // we give age(example 20) and hit enter, 
                                                // then it is taken as 20\n in input buffer.
                                                // after extracting 20, \n still is present in input buffer.
                                                // to clear it, we have to use std::ws or std::cin.ignore depending
                                                // on requirement. otherwise, \n is extracted for next cin.

    std::cout << "Enter your favorite color : ";
    std::string color{};
    std::getline(std::cin >> std::ws, color);   //  The std::ws input manipulator tells std::cin to ignore 
                                                //  any leading whitespace before extraction. 
                                                //  Leading whitespace is any whitespace character (spaces, tabs, newlines) 
                                                //  that occur at the start of the string.
    //std::getline(std::cin, color);

    std::cout << "Your name is " << name << ", your age is " << age <<" and your favorite color is " << color << '\n';

    return 0;
}