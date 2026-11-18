// 14_3_Const_class_objects_n_const_member_functions/11_avoid_const_nonconst_duplication.cpp
// Demonstrates how to avoid writing the same logic twice in a const and non-const overload pair.
//
// Idiom (Scott Meyers, Effective C++ Item 3): write the logic ONCE in the const version,
// and make the non-const version call it and cast the const away from the result.
//
//   T& get(i) { return const_cast<T&>(std::as_const(*this).get(i)); }
//
// Why this is safe: the non-const version can only be called on a non-const object,
// so the object we cast const away from was never really const.
// Never do the reverse (const version calling the non-const one): that would let a
// const object run code that is allowed to modify it.
//
// C++23 alternative: "deducing this" (explicit object parameter) writes ONE function:
//   auto& get(this auto& self, std::size_t i) { /* checks */ return self.m_data[i]; }
// self is deduced as Buffer& or const Buffer&, so the return type follows automatically.
// Requires GCC 14+ / Clang 18+ / MSVC 19.32+, so it is shown here only as a comment.
#include <array>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>   // std::as_const

class Buffer
{
    std::array<int, 4> m_data{ 10, 20, 30, 40 };

public:
    // The ONE place where the real logic lives (bounds check, logging, etc.)
    const int& at(std::size_t i) const
    {
        if (i >= m_data.size())
            throw std::out_of_range{ "Buffer::at" };
        return m_data[i];
    }

    // Non-const overload reuses the const one: no duplicated logic
    int& at(std::size_t i)
    {
        return const_cast<int&>(std::as_const(*this).at(i));
    }
};

int main()
{
    Buffer buf{};
    buf.at(1) = 99;                       // non-const overload: returns int&
    std::cout << buf.at(1) << '\n';       // 99

    const Buffer cbuf{};
    std::cout << cbuf.at(2) << '\n';      // const overload: returns const int&
    // cbuf.at(2) = 5;                    // ERROR: assignment of read-only location

    try { buf.at(10); }                   // bounds check written once, works for both
    catch (const std::out_of_range& e) { std::cout << "caught: " << e.what() << '\n'; }
    return 0;
}
