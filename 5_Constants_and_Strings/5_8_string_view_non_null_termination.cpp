// std::string_view can view a substring
// std::string_view may or may not be null-terminated


#include <iostream>

void print(std::string_view sv)
{
    
	std::cout << "size : " << std::ssize(sv) << "\n";
	std::cout << "size : " << sv.size() << "\n";
	std::cout << "length : " << sv.length() << "\n";

	//std::cout << sv << "\n"; // Undefined behavior: may print garbage after "Hello"
	std::cout.write(sv.data(), sv.size()) << std::endl; // Correct way to print std::string_view
}

int main()
{
    
    print(std::string{"Hello World", 0, 5});
    
    return 0;
}