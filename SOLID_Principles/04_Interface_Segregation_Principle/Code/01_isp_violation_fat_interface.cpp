/**
 * @file 01_isp_violation_fat_interface.cpp
 * @brief Demonstrates the Interface Segregation Principle (ISP) violation: The "Fat Interface".
 * 
 * In this anti-pattern, `IMultiFunctionDevice` aggregates disparate hardware operations:
 *   - Printing
 *   - Scanning
 *   - Faxing
 *   - Stapling
 *   - Cloud Upload
 * 
 * Why this fails in production C++:
 *   - Forced Dummy Implementations: A simple desktop inkjet printer or hand-held scanner
 *     is forced to provide empty stubs or throw exceptions for methods it cannot support physically.
 *   - Unnecessary Recompilation (Lakos Physical Coupling): When the `faxDocument()` signature changes
 *     (e.g., adding an international dialing code), all printers, scanners, and their callers
 *     must recompile, even though they never touch fax functionality.
 *   - Misleading API Contracts: Clients cannot trust the interface; any call might throw "Not Implemented".
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>

// ============================================================================
// ANTI-PATTERN: Fat Monolithic Interface
// ============================================================================
class IMultiFunctionDevice {
public:
    virtual ~IMultiFunctionDevice() = default;

    virtual void printDocument(const std::string& doc) = 0;
    virtual void scanDocument(const std::string& docPath) = 0;
    virtual void faxDocument(const std::string& doc, const std::string& phoneNumber) = 0;
    virtual void stapleDocument() = 0;
    virtual void cloudUpload(const std::string& doc) = 0;
};

// ============================================================================
// CONCRETE DEVICE 1: High-End Enterprise Xerox (Genuinely supports all features)
// ============================================================================
class EnterpriseXerox : public IMultiFunctionDevice {
public:
    void printDocument(const std::string& doc) override {
        std::cout << "[EnterpriseXerox] Printing 1200 DPI: " << doc << "\n";
    }
    void scanDocument(const std::string& docPath) override {
        std::cout << "[EnterpriseXerox] Scanning high-speed feeder to: " << docPath << "\n";
    }
    void faxDocument(const std::string& doc, const std::string& phoneNumber) override {
        std::cout << "[EnterpriseXerox] Dialing " << phoneNumber << " and faxing: " << doc << "\n";
    }
    void stapleDocument() override {
        std::cout << "[EnterpriseXerox] Stapling finished packet with mechanical arm.\n";
    }
    void cloudUpload(const std::string& doc) override {
        std::cout << "[EnterpriseXerox] Syncing " << doc << " to enterprise SharePoint.\n";
    }
};

// ============================================================================
// CONCRETE DEVICE 2: Basic Desktop Inkjet Printer
// Suffers from ISP Violation: Forced to implement functions it physically lacks!
// ============================================================================
class BasicDesktopPrinter : public IMultiFunctionDevice {
public:
    void printDocument(const std::string& doc) override {
        std::cout << "[BasicDesktopPrinter] Printing document: " << doc << "\n";
    }

    // --- Forced Dummy Implementations / ISP Violations ---
    void scanDocument(const std::string&) override {
        throw std::logic_error("[BasicDesktopPrinter] Error: Device has no optical scanner!");
    }

    void faxDocument(const std::string&, const std::string&) override {
        throw std::logic_error("[BasicDesktopPrinter] Error: Device has no modem/fax hardware!");
    }

    void stapleDocument() override {
        throw std::logic_error("[BasicDesktopPrinter] Error: Device has no physical stapler!");
    }

    void cloudUpload(const std::string&) override {
        throw std::logic_error("[BasicDesktopPrinter] Error: Device is USB only, no network card!");
    }
};

// Client that only cares about printing documents
void printReport(IMultiFunctionDevice& device, const std::string& report) {
    // Client is coupled to all 5 methods even though it only needs 1!
    device.printDocument(report);
}

int main() {
    std::cout << "=== ISP Violation: Fat Interface Antipattern ===\n\n";

    EnterpriseXerox enterpriseDevice;
    BasicDesktopPrinter homePrinter;

    std::cout << "--- 1. Printing on Enterprise Xerox ---\n";
    printReport(enterpriseDevice, "Q3 Financial Statement");

    std::cout << "\n--- 2. Printing on Basic Home Printer ---\n";
    printReport(homePrinter, "Homework Assignment");

    std::cout << "\n--- 3. Client Attempting to Fax on Basic Home Printer ---\n";
    try {
        homePrinter.faxDocument("Secret Memo", "+1-800-555-0199");
    } catch (const std::exception& ex) {
        std::cerr << "💥 Runtime Crash: " << ex.what() << "\n";
        std::cerr << "Reason: Fat interface forced client to believe the device could fax!\n";
    }

    return 0;
}
