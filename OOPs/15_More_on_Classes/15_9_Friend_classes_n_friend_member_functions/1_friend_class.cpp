#include <iostream>

class BankAccount
{
    friend class BalanceCheck;      // it doesn't matter if friend is declared under private, protected or public.

    int m_savingDeposit{};
    int m_fixedDeposit{};
public:
    BankAccount(int sd, int fd) : m_savingDeposit{sd}, m_fixedDeposit{fd}
    {
    }

    //friend class BalanceCheck;
};

class BalanceCheck
{
    bool m_istotalDepositsCheck{};
public:
    BalanceCheck(bool flag) : m_istotalDepositsCheck{flag}
    {
    }
    
    int getBankAccountDeposits(const BankAccount& acc) const
    {
        if(m_istotalDepositsCheck)
            return acc.m_savingDeposit + acc.m_fixedDeposit;

        return acc.m_savingDeposit;
    } 

    void setBalanceFlag(bool input)
    {
        m_istotalDepositsCheck = input;
    }
};

int main()
{
    BankAccount acc1{100000, 500000};
    BalanceCheck bc1{false};

    std::cout << "Total Account Deposits : " << bc1.getBankAccountDeposits(acc1) << "\n";

    bc1.setBalanceFlag(true);
    std::cout << "Total Account Deposits : " << bc1.getBankAccountDeposits(acc1) << "\n";
    
    return 0;
}