/**
 * @file 3_user_defined_literals_dimensional_analysis.cpp
 * @brief Demonstrates User-Defined Literals (UDLs) and Dimensional Analysis:
 *        - Defining custom literal operators (operator"" _suffix).
 *        - Compile-time conversion and constexpr arithmetic.
 *        - Enforcing compile-time type safety across physical units (meters, kilometers, centimeters).
 */

#include <iostream>

class Distance {
private:
    double m_meters{0.0};

    // Explicit constructor to prevent implicit conversion from raw double
    constexpr explicit Distance(double meters) : m_meters(meters) {}

public:
    constexpr double inMeters() const { return m_meters; }
    constexpr double inKilometers() const { return m_meters / 1000.0; }
    constexpr double inCentimeters() const { return m_meters * 100.0; }

    // Arithmetic operators
    constexpr Distance operator+(const Distance& other) const {
        return Distance(m_meters + other.m_meters);
    }

    constexpr Distance operator-(const Distance& other) const {
        return Distance(m_meters - other.m_meters);
    }

    // Friend literal operators
    friend constexpr Distance operator""_m(long double meters);
    friend constexpr Distance operator""_km(long double kilometers);
    friend constexpr Distance operator""_cm(long double centimeters);
};

// Cooked literal operators (must start with underscore)
constexpr Distance operator""_m(long double meters) {
    return Distance(static_cast<double>(meters));
}

constexpr Distance operator""_km(long double kilometers) {
    return Distance(static_cast<double>(kilometers) * 1000.0);
}

constexpr Distance operator""_cm(long double centimeters) {
    return Distance(static_cast<double>(centimeters) / 100.0);
}

int main() {
    std::cout << "=== User-Defined Literals (UDLs) ===\n";

    // Clean, self-documenting syntax with compile-time computation!
    constexpr Distance total = 2.5_km + 300.0_m + 50.0_cm;

    std::cout << "2.5_km + 300.0_m + 50.0_cm equals:\n";
    std::cout << "  In Meters:      " << total.inMeters() << " m\n";
    std::cout << "  In Kilometers:  " << total.inKilometers() << " km\n";
    std::cout << "  In Centimeters: " << total.inCentimeters() << " cm\n\n";

    std::cout << "=== Compile-Time Type Safety ===\n";
    std::cout << "Writing 'Distance d = 50.0;' will FAIL compilation (explicit constructor)!\n";
    std::cout << "Writing 'total + 10.0' will FAIL compilation (prevents unit mismatch bugs)!\n";

    return 0;
}
