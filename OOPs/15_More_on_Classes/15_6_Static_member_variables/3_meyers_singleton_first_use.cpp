// 15_6_Static_member_variables/3_meyers_singleton_first_use.cpp
// Demonstrates solving the Static Initialization Order Fiasco using the "Construct On First Use" idiom (Meyers' Singleton).

#include <iostream>
#include <string>

class DatabaseConnection
{
    std::string m_connectionString{};

    // Private constructor prevents direct unauthorized instantiation
    DatabaseConnection(std::string_view conn) : m_connectionString{conn}
    {
        std::cout << "DatabaseConnection initialized: " << m_connectionString << '\n';
    }

public:
    // Delete copy and move semantics for singleton integrity
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    // Meyers' Singleton: Construct-on-first-use pattern.
    // C++11 guarantees that function-local static variables are initialized in a thread-safe manner
    // precisely when control passes through their declaration for the first time.
    static DatabaseConnection& getInstance()
    {
        static DatabaseConnection s_instance{"db://production:5432"};
        return s_instance;
    }

    void query(std::string_view sql) const
    {
        std::cout << "Executing query on " << m_connectionString << ": " << sql << '\n';
    }
};

void runServiceA()
{
    std::cout << "Service A accessing DB...\n";
    DatabaseConnection::getInstance().query("SELECT * FROM users");
}

void runServiceB()
{
    std::cout << "Service B accessing DB...\n";
    DatabaseConnection::getInstance().query("SELECT * FROM orders");
}

int main()
{
    // First call instantiates s_instance exactly once:
    runServiceA();

    // Subsequent calls safely reuse the already initialized singleton:
    runServiceB();

    return 0;
}
