// 14_3_Const_class_objects_n_const_member_functions/5_mutable_data_members.cpp
// Demonstrates the use of the mutable keyword to allow modification of data members from const member functions.

#include <iostream>
#include <string>

class WebPage
{
    std::string m_title{};
    // mutable allows modification even when the enclosing object is const:
    mutable int m_viewCount{0};

public:
    WebPage(std::string_view title) : m_title{title} {}

    // A const member function guarantees logical constness to callers,
    // while updating internal non-observable state (like caches or telemetry/view counts):
    void display() const
    {
        ++m_viewCount; // OK: m_viewCount is mutable
        std::cout << "Displaying page: " << m_title << " (view count: " << m_viewCount << ")\n";
    }

    int getViewCount() const { return m_viewCount; }
};

int main()
{
    const WebPage home{"Home Page"}; // const object

    home.display();
    home.display();
    home.display();

    std::cout << "Total views recorded on const object: " << home.getViewCount() << '\n';

    return 0;
}
