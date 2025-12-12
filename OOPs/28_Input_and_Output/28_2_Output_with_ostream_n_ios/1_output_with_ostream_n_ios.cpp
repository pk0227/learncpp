// Demonstrates ostream formatting flags and manipulators
// Key concept: Most manipulators PERSIST across multiple output operations
// Use flags() to save/restore original formatting state

#include <iostream>
#include <iomanip>

int main()
{
    // Save original formatting flags to restore later
    auto oldFlags = std::cout.flags();          // auto deduces to std::ios_base::fmtflags
    
    // ===== EXAMPLE 1: std::showpos / std::noshowpos =====
    // Controls whether '+' sign is shown for positive numbers
    int i{123};
    std::cout << std::showpos << i << "\n";     // Output: +123 (showpos persists)
    std::cout << i << "\n";                      // Output: +123 (still has effect)
    std::cout << std::noshowpos;                 // Disable showpos
    std::cout << i << "\n";                      // Output: 123 (no '+' sign)
    
    std::cout.flags(oldFlags);  // Restore original flags
    std::cout << "================================================\n";
    
    // ===== EXAMPLE 2: std::boolalpha / std::noboolalpha =====
    // Controls whether bools print as "true"/"false" or "1"/"0"
    bool flag{true};
    std::cout << flag << "\n";                   // Output: 1 (default numeric format)
    std::cout << std::boolalpha << flag << "\n"; // Output: true (boolalpha persists)
    std::cout << flag << "\n";                   // Output: true (still has effect)
    std::cout << std::noboolalpha;               // Disable boolalpha
    std::cout << flag << "\n";                   // Output: 1 (back to numeric)

    std::cout.flags(oldFlags);
    std::cout << "================================================\n";
    
    // ===== EXAMPLE 3: std::hex / std::dec / std::oct =====
    // Controls numeric base for integer output (hexadecimal, decimal, octal)
    int val{27};
    std::cout << val << "\n";                    // Output: 27 (default decimal)
    std::cout << std::hex << val << "\n";        // Output: 1b (hex persists)
    std::cout << val << "\n";                    // Output: 1b (still hex)
    std::cout << std::dec;                       // Switch back to decimal
    std::cout << val << "\n";                    // Output: 27
    
    std::cout.flags(oldFlags);
    std::cout << "================================================\n";
    
    // ===== EXAMPLE 4: std::scientific / std::uppercase =====
    // scientific: forces exponential notation (e.g., 1.234e+02)
    // uppercase: uses uppercase 'E' instead of 'e'
    double d{123.456789123};
    std::cout << d << "\n";                      // Output: 123.457 (default format)
    std::cout << std::scientific << d << "\n";   // Output: 1.234568e+02 (scientific persists)
    std::cout << std::uppercase << d << "\n";    // Output: 1.234568E+02 (uppercase E, both persist)
    std::cout << d << "\n";                      // Output: 1.234568E+02 (both still active)
    std::cout << std::nouppercase;               // Disable uppercase
    std::cout << d << "\n";                      // Output: 1.234568e+02 (back to lowercase e)
    std::cout.unsetf(std::ios::scientific);      // Disable scientific notation
    std::cout << d << "\n";                      // Output: 123.457 (back to default)
    
    std::cout.flags(oldFlags);
    std::cout << "================================================\n";
    
    // ===== EXAMPLE 5: std::fixed with std::setprecision =====
    /*
        If fixed or scientific notation is used, precision determines how many decimal places in the fraction is displayed. 
        Note that if the precision is less than the number of significant digits, the number will be rounded.
    */
    
    double d1{123.456};
    std::cout << d1 << "\n";                                // Output: 123.456 (default)
    //std::cout << std::setprecision(7) << d1 << "\n";      // setprecision persists but only effective with fixed/scientific
    std::cout << std::fixed;                                // fixed persists, default precision is 6
    std::cout << d1 << "\n";                                // Output: 123.456000 (6 decimal places)
                                                            // If setprecision(7) uncommented: 123.4560000 (7 decimal places)
    std::cout << std::setprecision(3) << d1 << "\n";       // Output: 123.456
    std::cout << std::setprecision(4) << d1 << "\n";       // Output: 123.4560
    std::cout << std::setprecision(5) << d1 << "\n";       // Output: 123.45600
    std::cout << std::setprecision(6) << d1 << "\n";       // Output: 123.456000
    std::cout << std::setprecision(7) << d1 << "\n";       // Output: 123.4560000
    
    std::cout.flags(oldFlags);
    std::cout << "================================================\n";
    
    // ===== EXAMPLE 6: std::scientific with std::setprecision =====
    double d2{123.456};
    std::cout << d2 << "\n";                                // Output: 123.456
    //std::cout << std::setprecision(8) << d2 << "\n";      // setprecision persists but only effective with fixed/scientific
    std::cout << std::scientific;                           // scientific persists, default precision is 6 (same as fixed)
    std::cout << d2 << "\n";                                // Output: 1.234560e+02 (6 decimal places - default)
                                                            // If setprecision(8) uncommented: 1.23456000e+02 (8 decimal places)
    std::cout << std::setprecision(3) << d2 << "\n";       // Output: 1.235e+02
    std::cout << std::setprecision(4) << d2 << "\n";       // Output: 1.2346e+02
    std::cout << std::setprecision(5) << d2 << "\n";       // Output: 1.23456e+02
    std::cout << std::setprecision(6) << d2 << "\n";       // Output: 1.234560e+02
    std::cout << std::setprecision(7) << d2 << "\n";       // Output: 1.2345600e+02
    
    std::cout.flags(oldFlags);
    std::cout << "================================================\n";
    
    // ===== EXAMPLE 7: Precision without fixed/scientific (significant digits) =====
    /*
        If neither fixed nor scientific are being used, precision determines how many significant digits should be displayed. 
        Again, if the precision is less than the number of significant digits, the number will be rounded.
    */
    double d3{123.456};
    std::cout << d3 << "\n";                     // Output: 123.456 (default 6 significant digits)
    
    std::cout << std::showpoint << '\n';         // showpoint: always show decimal point and trailing zeros
    std::cout << std::setprecision(3) << d3 << '\n';  // Output: 123. (3 significant digits, rounded)
    std::cout << std::setprecision(4) << d3 << '\n';  // Output: 123.5 (4 significant digits)
    std::cout << std::setprecision(5) << d3 << '\n';  // Output: 123.46 (5 significant digits)
    std::cout << std::setprecision(6) << d3 << '\n';  // Output: 123.456 (6 significant digits)
    std::cout << std::setprecision(7) << d3 << '\n';  // Output: 123.4560 (7 significant digits, trailing zero)
    
    std::cout.flags(oldFlags);
    std::cout << "================================================\n";
    
    // ===== EXAMPLE 8: std::setw (field width) with alignment =====
    // setw: sets minimum field width (DOES NOT PERSIST - must be set for each output)
    // Alignment manipulators (right/left/internal) DO PERSIST
    int val1{-12345};
    std::cout << std::right;                            // Right-align (default), persists
    std::cout << std::setw(10) << val1 << "\n";        // Output: "    -12345" (right-aligned in 10 chars)
    std::cout << std::setw(10) << val1 << "\n";        // Output: "    -12345" (setw reapplied, right still active)
    
    std::cout << std::setw(10) << std::left << val1 << "\n";     // Output: "-12345    " (left-aligned)
    std::cout << std::setw(10) << std::internal << val1 << "\n"; // Output: "-    12345" (sign left, number right)
    
    std::cout << "----------------\n";
    
    // ===== EXAMPLE 9: std::fill (fill character) =====
    // fill: sets character to use for padding (PERSISTS)
    std::cout.fill('*');                                 // Use '*' for padding, persists
    std::cout << std::setw(10) << std::left << val1 << "\n";     // Output: "-12345****"
    std::cout << std::setw(10) << std::right << val1 << "\n";    // Output: "****-12345"
    std::cout << std::setw(10) << std::internal << val1 << "\n"; // Output: "-****12345"

    std::cout.flags(oldFlags);
    std::cout << "================================================\n";
    
    return 0;
}