#include <iostream>
#include <string>

std::string getName() {
    std::string name{};
    std::cout << "Enter Name : ";
    std::cin >> name;
    std::cout << "addr of name : " << &name << std::endl;
    return name; // RVO or move semantics can optimize this
}

int main() {
    std::string u_name{getName()};  // RVO or move semantics
    //std::string u_name{};
    //u_name =  getName();
    std::cout << "Your name is " << u_name << "\naddr of u_name : " << &u_name << std::endl;
    return 0;
}
