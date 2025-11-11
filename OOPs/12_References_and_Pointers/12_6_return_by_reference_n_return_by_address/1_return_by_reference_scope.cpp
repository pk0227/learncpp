#include <iostream>

const std::string& getAppName()
{
    static const std::string s_appName{"Calculator"};   // has static storage duration, so it lives until program ends
    return s_appName;
}

int main()
{
    std::cout << "App name is " << getAppName() << "\n";
    return 0;
}