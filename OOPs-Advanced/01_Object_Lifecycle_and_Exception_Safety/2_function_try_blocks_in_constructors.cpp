/**
 * @file 2_function_try_blocks_in_constructors.cpp
 * @brief Demonstrates Function-Try-Blocks on Class Constructors:
 *        - Wrapping the member initializer list to catch sub-object exceptions.
 *        - The mandatory implicit rethrow rule (constructors cannot suppress exceptions).
 *        - The danger/UB of accessing member variables inside the catch block.
 */

#include <iostream>
#include <stdexcept>
#include <string>

class SubSystemA {
public:
    SubSystemA(const std::string& name) {
        std::cout << "  SubSystemA [" << name << "] constructed.\n";
    }
    ~SubSystemA() {
        std::cout << "  SubSystemA destructed.\n";
    }
};

class SubSystemB {
public:
    SubSystemB(bool fail) {
        if (fail) {
            std::cout << "  SubSystemB: Failure during initialization!\n";
            throw std::runtime_error("SubSystemB failed to initialize");
        }
        std::cout << "  SubSystemB constructed.\n";
    }
    ~SubSystemB() {
        std::cout << "  SubSystemB destructed.\n";
    }
};

class Controller {
private:
    SubSystemA m_subA;
    SubSystemB m_subB;

public:
    // Function-try-block wrapping the member initializer list:
    Controller(bool failB) try
        : m_subA("Network"),
          m_subB(failB)
    {
        std::cout << "Controller: Constructor body reached.\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "Controller catch block: Caught exception: " << e.what() << "\n";
        std::cout << "Controller catch block: Note that m_subA has ALREADY been destroyed by stack unwinding!\n";
        std::cout << "Controller catch block: Exiting catch block without explicit throw...\n";
        // The standard mandates: reaching the end of a constructor function-try-block
        // automatically executes an implicit 'throw;'!
    }

    ~Controller() {
        std::cout << "Controller destructed.\n";
    }
};

int main() {
    std::cout << "=== Test 1: Successful Controller Construction ===\n";
    try {
        Controller c(false);
        std::cout << "Controller is operational.\n";
    } catch (const std::exception& e) {
        std::cout << "Unexpected failure: " << e.what() << "\n";
    }

    std::cout << "\n=== Test 2: SubSystemB Failure with Function-Try-Block ===\n";
    try {
        Controller c(true);
        std::cout << "This line will NEVER be reached.\n";
    } catch (const std::exception& e) {
        std::cout << "Main: Successfully caught propagated exception: " << e.what() << "\n";
        std::cout << "Proof: Function-try-block in constructor CANNOT suppress exceptions!\n";
    }

    return 0;
}
