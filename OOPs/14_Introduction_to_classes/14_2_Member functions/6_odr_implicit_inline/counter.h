#ifndef COUNTER_H
#define COUNTER_H

// This header is #included by BOTH translation units (file_a.cpp and file_b.cpp).
//
// KEY POINT (ODR):
// A member function DEFINED INSIDE the class definition is implicitly `inline`.
// `inline` here does NOT mean "expand at the call site" -- in the ODR sense it
// means "this definition may appear in multiple translation units, and the
// linker will treat them as THE SAME single definition (and de-duplicate them)".
//
// Because of this, including this header into two .cpp files and linking them
// together does NOT violate the One Definition Rule -- even though `increment()`
// and `get()` are textually defined in every file that includes the header.

class Counter
{
    int m_count{ 0 };

public:
    // Defined INSIDE the class -> implicitly inline -> ODR-safe across TUs.
    void increment() { ++m_count; }
    int  get() const { return m_count; }
};

// For contrast: a NON-member function defined in a header is NOT implicitly
// inline. If we removed the `inline` keyword below, including this header into
// two .cpp files would cause a "multiple definition" link error. The explicit
// `inline` is what the compiler adds for us automatically on in-class members.
inline const char* label() { return "Counter"; }

#endif // COUNTER_H
