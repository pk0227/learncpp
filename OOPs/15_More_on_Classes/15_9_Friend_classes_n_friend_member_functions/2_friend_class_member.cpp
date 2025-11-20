#include <iostream>

/*
    When making a specific member function a friend (not the entire class), follow this order:
    
    1. Forward declare the class that will contain the private members (BankAccount)
    2. Fully define the class with the friend function (BalanceCheck) - but only DECLARE the friend function
    3. Fully define the class with private members (BankAccount) and add the friend declaration
    4. DEFINE the friend function after BankAccount is complete
    
    Why this order?
    - The friend declaration in BankAccount needs to know BalanceCheck exists and has that specific member function
    - The friend function definition needs to access BankAccount's private members, so BankAccount must be fully defined
    - If we define the function inside BalanceCheck class, BankAccount is still incomplete (forward declared only)
*/

// Step 1: Forward declare BankAccount so BalanceCheck can reference it
class BankAccount;

// Step 2: Define BalanceCheck class with function DECLARATION only
class BalanceCheck
{
    bool m_istotalDepositsCheck{};
public:
    BalanceCheck(bool flag) : m_istotalDepositsCheck{flag}
    {
    }
    
    // Only declare the function here - don't define it yet
    // (BankAccount is still incomplete at this point)
    int getBankAccountDeposits(const BankAccount& acc) const;

    void setBalanceFlag(bool input)
    {
        m_istotalDepositsCheck = input;
    }
};

// Step 3: Define BankAccount with the friend declaration
class BankAccount
{
    // Making a specific member function a friend (not the whole class)
    // Note: friend declarations can be in private, protected, or public - doesn't matter
    friend int BalanceCheck::getBankAccountDeposits(const BankAccount& acc) const;

    int m_savingDeposit{};
    int m_fixedDeposit{};
public:
    BankAccount(int sd, int fd) : m_savingDeposit{sd}, m_fixedDeposit{fd}
    {
    }

    // Alternative: Making the entire class a friend would look like this:
    // friend class BalanceCheck;
};

// Step 4: Define the friend function AFTER BankAccount is fully defined
// Now the function can access BankAccount's private members because:
// 1. BankAccount is complete, so the compiler knows its members
// 2. This function is declared as a friend, so it has access to private members
int BalanceCheck::getBankAccountDeposits(const BankAccount& acc) const
{
    if(m_istotalDepositsCheck)
        return acc.m_savingDeposit + acc.m_fixedDeposit;  // Access private members

    return acc.m_savingDeposit;  // Access private member
}


int main()
{
    BankAccount acc1{100000, 500000};
    BalanceCheck bc1{false};

    std::cout << "Total Account Deposits : " << bc1.getBankAccountDeposits(acc1) << "\n";

    bc1.setBalanceFlag(true);
    std::cout << "Total Account Deposits : " << bc1.getBankAccountDeposits(acc1) << "\n";
    
    return 0;
}