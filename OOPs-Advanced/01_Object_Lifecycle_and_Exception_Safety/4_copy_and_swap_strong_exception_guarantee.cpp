/**
 * @file 4_copy_and_swap_strong_exception_guarantee.cpp
 * @brief Demonstrates the Copy-and-Swap Idiom:
 *        - Non-member friend swap using ADL.
 *        - Unified assignment operator passing parameter by value.
 *        - Strong Exception Guarantee and automatic self-assignment safety.
 */

#include <iostream>
#include <cstring>
#include <utility>

class SafeString {
private:
    char* m_data{nullptr};
    std::size_t m_size{0};

public:
    // Default constructor
    SafeString() = default;

    // Value constructor
    SafeString(const char* str) {
        if (str) {
            m_size = std::strlen(str);
            m_data = new char[m_size + 1];
            std::memcpy(m_data, str, m_size + 1);
        }
        std::cout << "  Constructed SafeString: \"" << (m_data ? m_data : "") << "\"\n";
    }

    // Copy constructor
    SafeString(const SafeString& other) : m_size(other.m_size) {
        if (other.m_data) {
            m_data = new char[m_size + 1];
            std::memcpy(m_data, other.m_data, m_size + 1);
        }
        std::cout << "  Copy-constructed SafeString from: \"" << (m_data ? m_data : "") << "\"\n";
    }

    // Move constructor (noexcept!)
    SafeString(SafeString&& other) noexcept 
        : m_data(std::exchange(other.m_data, nullptr)),
          m_size(std::exchange(other.m_size, 0)) 
    {
        std::cout << "  Move-constructed SafeString (stole buffer): \"" << (m_data ? m_data : "") << "\"\n";
    }

    // Destructor
    ~SafeString() {
        std::cout << "  Destructing SafeString: \"" << (m_data ? m_data : "null") << "\"\n";
        delete[] m_data;
    }

    // Friend swap using ADL (Argument-Dependent Lookup)
    friend void swap(SafeString& first, SafeString& second) noexcept {
        using std::swap;
        swap(first.m_data, second.m_data);
        swap(first.m_size, second.m_size);
    }

    // Unified Assignment Operator (Pass by Value!)
    SafeString& operator=(SafeString other) noexcept {
        std::cout << "  operator=(SafeString other) entered, swapping...\n";
        swap(*this, other);
        // When 'other' goes out of scope here, its destructor frees the OLD buffer!
        return *this;
    }

    const char* c_str() const { return m_data ? m_data : ""; }
};

int main() {
    std::cout << "=== 1. Copy Assignment via Copy-and-Swap ===\n";
    {
        SafeString str1("Original Destination");
        SafeString str2("Source Value");
        std::cout << "Executing: str1 = str2;\n";
        str1 = str2; // Invokes copy ctor to initialize parameter 'other', then swaps
        std::cout << "str1 after assignment: \"" << str1.c_str() << "\"\n";
    }

    std::cout << "\n=== 2. Move Assignment via Copy-and-Swap ===\n";
    {
        SafeString str1("Target To Overwrite");
        SafeString str2("Temporary Resource");
        std::cout << "Executing: str1 = std::move(str2);\n";
        str1 = std::move(str2); // Invokes move ctor to initialize parameter 'other', then swaps
        std::cout << "str1 after move-assignment: \"" << str1.c_str() << "\"\n";
        std::cout << "str2 after move: \"" << str2.c_str() << "\"\n";
    }

    std::cout << "\n=== 3. Self-Assignment Safety ===\n";
    {
        SafeString str1("Self-Assign Me");
        std::cout << "Executing: str1 = str1;\n";
        str1 = str1; // Safe! Makes a copy, swaps, and destroys copy
        std::cout << "str1 after self-assignment: \"" << str1.c_str() << "\"\n";
    }

    return 0;
}
