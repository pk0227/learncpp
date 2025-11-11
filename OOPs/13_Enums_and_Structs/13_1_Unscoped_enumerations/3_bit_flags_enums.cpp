#include <bitset>
#include <iostream>

namespace Flags
{
    enum State
    {
        isHungry,
        isSad,
        isMad,
        isHappy,
        isLaughing,
        isAsleep,
        isDead,
        isCrying,
    };
}

int main()
{
    std::bitset<8> me{};
    me.set(Flags::isHappy);                 // Flags::isHappy equals to Flags::State::isHappy
    me.set(Flags::State::isLaughing);   

    std::cout << std::boolalpha; // print bool as true/false

    // Query a few states (we use the any() function to see if any bits remain set)
    std::cout << "I am happy? " << me.test(Flags::State::isHappy) << '\n';
    std::cout << "I am laughing? " << me.test(Flags::isLaughing) << '\n';

    return 0;
}