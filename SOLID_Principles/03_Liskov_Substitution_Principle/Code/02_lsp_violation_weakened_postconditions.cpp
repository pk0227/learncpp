/**
 * @file 02_lsp_violation_weakened_postconditions.cpp
 * @brief Demonstrates Advanced C++ LSP Violations: Contract Weakening, Exceptions, and Slicing.
 * 
 * Formal Liskov Behavioral Subtyping Rules:
 *   1. Preconditions cannot be strengthened in a subtype (Contravariance of inputs).
 *   2. Postconditions cannot be weakened in a subtype (Covariance of outputs).
 *   3. Invariants of the supertype must be preserved in all subtypes.
 *   4. Exception safety: A subtype cannot throw checked/unexpected exceptions not permitted by supertype.
 *   5. Object Slicing: C++ specific bug where passing polymorphic types by-value destroys subtype state.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <vector>

// ============================================================================
// VIOLATION 1: The "Read-Only" Resource Trap (Throwing Unexpected Exceptions)
// Base class promises a read/write contract. Subtype rejects writes!
// ============================================================================
class FileStream {
public:
    virtual ~FileStream() = default;

    virtual void write(const std::string& data) {
        std::cout << "[FileStream] Writing " << data.size() << " bytes to disk.\n";
    }

    [[nodiscard]] virtual std::string read(std::size_t bytes) {
        return std::string(bytes, 'A');
    }
};

class ReadOnlyFileStream : public FileStream {
public:
    // VIOLATION: Strengthens preconditions / throws unexpected exception!
    // Caller expecting FileStream never anticipated write() throwing UnsupportedOperation!
    void write(const std::string&) override {
        throw std::logic_error("LSP VIOLATION: Cannot write to a ReadOnlyFileStream!");
    }
};

void syncBufferToStorage(FileStream& fs, const std::string& payload) {
    // Client depends on FileStream write contract
    fs.write(payload);
    std::cout << "Data synchronized successfully.\n";
}

// ============================================================================
// VIOLATION 2: C++ Object Slicing
// Passing polymorphic types by value strips derived vtable & member data!
// ============================================================================
class BaseSensor {
public:
    virtual ~BaseSensor() = default;
    [[nodiscard]] virtual std::string getReading() const {
        return "Raw Voltage: 3.3V";
    }
};

class CalibratedSensor : public BaseSensor {
private:
    double calibrationOffset_{1.05};

public:
    [[nodiscard]] std::string getReading() const override {
        return "Calibrated Temperature: 24.5 C (Scale Factor: 1.05)";
    }
};

// ❌ SLICING TRAP: Passed by value!
void printSensorValueByValue(BaseSensor sensor) {
    std::cout << "[Slicing Trap] " << sensor.getReading() << "\n";
}

// ✅ CORRECT: Passed by const reference (Preserves polymorphic identity)
void printSensorValueByRef(const BaseSensor& sensor) {
    std::cout << "[Polymorphic Ref] " << sensor.getReading() << "\n";
}

// ============================================================================
// DEMO
// ============================================================================
int main() {
    std::cout << "=== Advanced LSP Violations in Production C++ ===\n\n";

    // Demo 1: Read-Only Resource Trap
    std::cout << "--- 1. Testing Read-Only Resource Subtyping ---\n";
    FileStream standardFile;
    syncBufferToStorage(standardFile, "System Transaction Log Entry");

    ReadOnlyFileStream readOnlyFile;
    try {
        std::cout << "Substituting ReadOnlyFileStream into FileStream reference...\n";
        syncBufferToStorage(readOnlyFile, "Another Log Entry");
    } catch (const std::exception& ex) {
        std::cerr << "💥 Caught Exception: " << ex.what() << "\n";
        std::cerr << "Explanation: Subtype broke caller's assumption that FileStream is writable!\n";
    }

    // Demo 2: C++ Object Slicing
    std::cout << "\n--- 2. Object Slicing on Pass-by-Value ---\n";
    CalibratedSensor advancedSensor;

    std::cout << "Calling via const BaseSensor&: ";
    printSensorValueByRef(advancedSensor); // Prints CalibratedSensor output

    std::cout << "Calling via BaseSensor (by-value): ";
    printSensorValueByValue(advancedSensor); // SLICED! Prints BaseSensor output!

    return 0;
}
