#include <iostream>

class Base
{
public:
    int m_public{};
protected:
    int m_protected{};
private:
    int m_private{};
};

class D1 : private Base
{
public:
    int m_public1{};
protected:
    int m_protected1{};
private:
    int m_private1{};
};

class D2 : public D1
{
public:
    int m_public2{};
protected:
    int m_protected2{};
private:
    int m_private2{};

public:
    D2()
    {
        // Privates of Base and D1 are inaccessible to D2 as D1 is privately inherited from Base. 
        //m_public = 1;
        //m_protected = 1;
        //m_private = 1;
        //m_private1 = 1;

        // public and protected of D1 are accessible to D2 due to public inheritance.
        m_public1 = 1;
        m_protected1 = 1;

        // D2 is always access its own members.
        m_public2 = 1;
        m_protected2 = 1;
        m_private2 = 1;
    }
};

int main()
{
    D2 d{};
    d.m_public1 = 2;
    d.m_public2 = 2;

    return 0;
}