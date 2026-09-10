#include <cassert> // for assert()
#include <iterator> // for std::size()

class IntList
{
private:
    int m_list[10]{};

public:
    int& operator[] (int index)
    {
        return m_list[index];
    }
};

int main()
{
    IntList* list{ new IntList{} };

    // PITFALL:
    // list[2] = 3; // COMPILE ERROR: list is a pointer, so list[2] computes *(list + 2),
                   // treating list as an array of IntList objects, not invoking operator[]!

    // CORRECT SYNTAX:
    // Dereference the pointer first, then invoke operator[]:
    (*list)[2] = 3;

    delete list;

    return 0;
}