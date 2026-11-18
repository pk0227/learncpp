// 14_3_Const_class_objects_n_const_member_functions/10_forcing_const_overload_as_const.cpp
// Demonstrates overload resolution between const and non-const member functions,
// and how to force the const overload from a NON-const object.
//
// - A non-const object always picks the non-const overload (exact match, no qualification added).
// - A const object can only pick the const overload.
// - To call the const overload on a non-const object, view it through a const reference:
//     std::as_const(obj)            (C++17, <utility>)  <- preferred, intent is obvious
//     static_cast<const T&>(obj)    (any standard)
// - std::as_const is deleted for rvalues, so it cannot create a dangling reference to a temporary.
#include <iostream>
#include <utility>   // std::as_const

class Document
{
public:
    void view()       { std::cout << "non-const view(): may modify (e.g. mark as read)\n"; }
    void view() const { std::cout << "const view(): read-only\n"; }
};

int main()
{
    Document doc{};
    const Document cdoc{};

    doc.view();                                  // non-const overload
    cdoc.view();                                 // const overload

    std::as_const(doc).view();                   // const overload on a non-const object (C++17)
    static_cast<const Document&>(doc).view();    // same, pre-C++17 spelling

    // std::as_const(Document{}).view();         // ERROR: as_const(const T&&) is deleted
    return 0;
}
