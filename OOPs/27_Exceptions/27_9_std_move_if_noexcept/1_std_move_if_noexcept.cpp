// Demonstrates std::move vs std::move_if_noexcept and exception safety
// std::move_if_noexcept uses move semantics only if move constructor is noexcept,
// otherwise falls back to copy to maintain exception safety

#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>
#include <utility>

// Class with noexcept move constructor - safe to move
class MoveClass
{
    int *m_resource{};  // Dynamically allocated resource
public:
    MoveClass(int value=0) : m_resource{new int{value}}
    { }

    // Copy constructor - deep copy of resource
    MoveClass(const MoveClass& ref)
    {
        if(&ref != this)
        {
            m_resource = new int{*ref.m_resource};
        }
    }

    // ✅ Move constructor marked noexcept - guarantees no exceptions
    // This makes MoveClass safe for std::move_if_noexcept
    MoveClass(MoveClass&& ref) noexcept : m_resource{ref.m_resource}
    {
        ref.m_resource = nullptr;  // Transfer ownership, leave source empty
    }

    ~MoveClass()
    {
        std::cout << "destroying " << *this << "\n";
        delete m_resource;
    }

    friend std::ostream& operator<<(std::ostream& out, const MoveClass& ref)
    {
        out << "MoveClass(" << (ref.m_resource ? std::to_string(*ref.m_resource) : "empty") << ")";
        return out;
    }
};

// Class with potentially throwing copy constructor and NO move constructor
// Without noexcept move constructor, std::move_if_noexcept will use copy instead
class CopyClass
{
public:
    bool m_throw{};  // Flag to control exception throwing

    CopyClass() = default;

    // ⚠️ Copy constructor that may throw (NOT noexcept)
    CopyClass(const CopyClass& ref) : m_throw{ref.m_throw}
    {
        if(m_throw)
            throw std::runtime_error{"abort!"};  // Simulates copy failure
    }
};

int main()
{
    // Create pair with MoveClass (noexcept move) and CopyClass (no move constructor)
    std::pair my_pair{MoveClass{50}, CopyClass{}};
    std::cout << "my_pair.first : " << my_pair.first << "\n";
    
    try
    {
        my_pair.second.m_throw = true;  // Enable exception throwing in CopyClass
        
        // ⚠️ Using std::move - UNCONDITIONAL move
        // Problem: MoveClass is moved (leaves my_pair.first empty)
        // Then CopyClass copy throws exception
        // Result: Exception thrown, but my_pair.first is already moved from!
        // my_pair is left in a PARTIALLY MOVED state (first is empty, second unchanged)
        std::pair moved_pair{std::move(my_pair)};
        
        // ✅ Alternative: std::move_if_noexcept - CONDITIONAL move
        // Behavior:
        // - If move constructor is noexcept → returns rvalue reference (uses move)
        // - If move constructor is NOT noexcept → returns lvalue reference (uses copy)
        // 
        // For std::pair:
        // - MoveClass: Has noexcept move → std::move_if_noexcept returns rvalue → moves
        // - CopyClass: No noexcept move → std::move_if_noexcept returns lvalue → copies
        // 
        // Result: Identical to std::move if all types have noexcept move constructors,
        // otherwise uses copy for safety (strong exception guarantee)
        //std::pair moved_pair{std::move_if_noexcept(my_pair)};

        std::cout << "moved_pair exists...!\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error : " << e.what() << "\n";
    }

    // 🔍 Check the state after exception:
    // With std::move: my_pair.first is EMPTY (MoveClass(empty)) - data lost!
    // With std::move_if_noexcept: my_pair.first is INTACT (MoveClass(50)) - data preserved!
    std::cout << "my_pair.first : " << my_pair.first << "\n";

    return 0;
}