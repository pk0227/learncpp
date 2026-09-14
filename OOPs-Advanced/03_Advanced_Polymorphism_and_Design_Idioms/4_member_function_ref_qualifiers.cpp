/**
 * @file 4_member_function_ref_qualifiers.cpp
 * @brief Demonstrates Member Function Ref-Qualifiers (& and &&):
 *        - Restricting member function invocation to lvalues or rvalues.
 *        - Preventing useless and dangerous mutations on temporary rvalues.
 *        - Move-optimizing getters to steal internal data from temporaries.
 */

#include <iostream>
#include <vector>
#include <string>

class HeavyDocument {
private:
    std::string m_name;
    std::vector<int> m_payload;

public:
    HeavyDocument(std::string name, std::size_t size) 
        : m_name(std::move(name)), m_payload(size, 42) {}

    // 1. Ref-Qualified Getter for LVALUES (&)
    // When called on an lvalue, returning by value would cause an expensive copy.
    // Return const reference instead!
    const std::vector<int>& getPayload() const & {
        std::cout << "  getPayload() const & called [LVALUE: returning const reference, ZERO copy]\n";
        return m_payload;
    }

    // 2. Ref-Qualified Getter for RVALUES (&&)
    // When called on a temporary rvalue, nobody else can use this object.
    // Steal its payload via std::move!
    std::vector<int> getPayload() && {
        std::cout << "  getPayload() && called [RVALUE: moving data out, ZERO copy resource theft!]\n";
        return std::move(m_payload);
    }

    // 3. Mutating Member Restricted to LVALUES (&)
    HeavyDocument& appendData(int val) & {
        m_payload.push_back(val);
        std::cout << "  appendData() & called on lvalue.\n";
        return *this;
    }

    // Deliberately NO appendData() && overload!
    // Calling .appendData() on a temporary would mutate a dying object and discard the result.
};

// Factory producing a temporary rvalue
HeavyDocument createDocument() {
    return HeavyDocument("TemporaryReport", 1000);
}

int main() {
    std::cout << "=== 1. Invocation on Lvalue ===\n";
    HeavyDocument persistentDoc("SavedReport", 500);
    const auto& ref = persistentDoc.getPayload(); // Calls const & overload
    persistentDoc.appendData(99);                 // Allowed on lvalue!

    std::cout << "\n=== 2. Invocation on Temporary Rvalue ===\n";
    // Calls && overload! Moves the 1000-element vector directly into stolenData!
    std::vector<int> stolenData = createDocument().getPayload();
    std::cout << "Stolen data elements: " << stolenData.size() << "\n";

    std::cout << "\n=== 3. Compilation Protection ===\n";
    std::cout << "Attempting createDocument().appendData(10) would FAIL compilation!\n";
    std::cout << "Because appendData() is qualified with '&', it rejects temporary rvalues!\n";

    return 0;
}
