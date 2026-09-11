/**
 * @file 2_dependencies.cpp
 * @brief Demonstrates the Dependency relationship in C++ and contrasts with Association.
 *
 * Concepts covered:
 * 1. Association: Object stores a persistent pointer/reference to another object ("uses-a").
 * 2. Dependency: Object relies on another object temporarily via function parameters, local variables,
 *    or return values ("depends-on"), without storing persistent members.
 * 3. Coupling implications: Dependencies create transient relationships and looser coupling.
 */

#include <iostream>
#include <string>
#include <string_view>

// Forward declaration of Point
class Point;

// A standalone Printer service class
class Printer
{
public:
    void printMessage(std::string_view msg) const
    {
        std::cout << "[Printer] " << msg << '\n';
    }
};

// 1. Dependency Example:
// Point depends on std::ostream and Printer, but does NOT store them as members!
class Point
{
    int m_x{};
    int m_y{};

public:
    Point(int x, int y) : m_x{x}, m_y{y} {}

    // Dependency on std::ostream via parameter
    friend std::ostream& operator<<(std::ostream& out, const Point& p)
    {
        out << "Point(" << p.m_x << ", " << p.m_y << ")";
        return out;
    }

    // Dependency on Printer via parameter
    void logWith(const Printer& printer) const
    {
        // Transient usage of Printer service
        printer.printMessage("Logging point coordinates: " + std::to_string(m_x) + ", " + std::to_string(m_y));
    }
};

// 2. Association Example:
// Doctor and Patient maintain persistent pointers/references to each other ("uses-a")
class Patient;

class Doctor
{
    std::string m_name;
    const Patient* m_currentPatient{nullptr}; // Association: persistent member pointer!

public:
    explicit Doctor(std::string_view name) : m_name{name} {}

    void assignPatient(const Patient* p) { m_currentPatient = p; }
    std::string_view getName() const { return m_name; }
};

int main()
{
    std::cout << "--- 1. Dependency Demonstration ---\n";
    Point pt{10, 20};
    Printer myPrinter;

    // pt uses myPrinter transiently; pt has no persistent reference or pointer to myPrinter
    pt.logWith(myPrinter);
    std::cout << "Direct stream dependency: " << pt << "\n\n";

    std::cout << "--- 2. Association Demonstration ---\n";
    Doctor doc{"Dr. Watson"};
    // Doctor holds persistent member pointing to patient during consultation
    std::cout << doc.getName() << " is on duty ready for associations.\n";

    return 0;
}
