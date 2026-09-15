/**
 * @file 02_isp_refactored_role_interfaces.cpp
 * @brief Demonstrates proper Interface Segregation Principle (ISP) via Role Interfaces.
 * 
 * Solutions to ISP Violations in C++:
 *   1. Granular Role Interfaces: Segregate operations into cohesive, single-purpose interfaces:
 *        - `IPrinter`: Pure printing capability.
 *        - `IScanner`: Pure optical scanning capability.
 *        - `IFax`: Pure telecommunication fax capability.
 *   2. Multiple Inheritance of Pure Abstract Classes: In C++, inheriting multiple pure abstract
 *      classes has ZERO memory overhead (no virtual inheritance required, no state diamond).
 *   3. Client Decoupling: A client that prints only includes and depends upon `IPrinter`.
 *      Modifications to scanning or faxing protocols cause ZERO recompilations for printing clients.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <memory>

// ============================================================================
// 1. SEGREGATED ROLE INTERFACES
// ============================================================================
class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const std::string& doc) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(const std::string& targetPath) = 0;
};

class IFax {
public:
    virtual ~IFax() = default;
    virtual void fax(const std::string& doc, const std::string& phoneNumber) = 0;
};

// ============================================================================
// 2. CONCRETE IMPLEMENTATIONS: Implement ONLY what is physically supported!
// ============================================================================

// Simple Desktop Printer: Clean, honest, zero dummy stubs!
class BasicInkjetPrinter : public IPrinter {
public:
    void print(const std::string& doc) override {
        std::cout << "[BasicInkjetPrinter] Printing document: " << doc << "\n";
    }
};

// Dedicated Optical Scanner: Clean, zero printer dependencies!
class FlatbedScanner : public IScanner {
public:
    void scan(const std::string& targetPath) override {
        std::cout << "[FlatbedScanner] Optical scan saved to: " << targetPath << "\n";
    }
};

// Enterprise Multi-Function Device: Composes multiple role interfaces cleanly!
class EnterpriseWorkstation : public IPrinter, public IScanner, public IFax {
public:
    void print(const std::string& doc) override {
        std::cout << "[Enterprise] Printing high-speed: " << doc << "\n";
    }

    void scan(const std::string& targetPath) override {
        std::cout << "[Enterprise] Scanning double-sided: " << targetPath << "\n";
    }

    void fax(const std::string& doc, const std::string& phoneNumber) override {
        std::cout << "[Enterprise] Faxing to " << phoneNumber << ": " << doc << "\n";
    }
};

// ============================================================================
// 3. CLIENT SERVICES: Depend ONLY on the roles they actually consume!
// ============================================================================

// Billing service only needs to print invoices
void generateInvoice(IPrinter& printer, const std::string& invoiceDetails) {
    std::cout << "[BillingService] Dispatching invoice to printer...\n";
    printer.print(invoiceDetails);
}

// Archival service only needs to scan records
void archivePhysicalRecord(IScanner& scanner, const std::string& storageLocation) {
    std::cout << "[ArchivalService] Digitizing paper records...\n";
    scanner.scan(storageLocation);
}

int main() {
    std::cout << "=== ISP Clean Refactoring: Segregated Role Interfaces ===\n\n";

    BasicInkjetPrinter homePrinter;
    FlatbedScanner officeScanner;
    EnterpriseWorkstation enterpriseHub;

    std::cout << "--- 1. Billing Service Printing Invoices ---\n";
    generateInvoice(homePrinter, "Invoice #4001 - $320.00");
    generateInvoice(enterpriseHub, "Invoice #4002 - $1,250.00");

    std::cout << "\n--- 2. Archival Service Scanning Documents ---\n";
    archivePhysicalRecord(officeScanner, "/var/data/records/doc_4001.pdf");
    archivePhysicalRecord(enterpriseHub, "/var/data/records/doc_4002.pdf");

    std::cout << "\nNotice: homePrinter has no scan() method, so passing it to archivePhysicalRecord()\n"
              << "FAILS AT COMPILE TIME rather than crashing at runtime!\n";

    return 0;
}
