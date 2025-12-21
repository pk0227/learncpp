/*
 * Demonstration: DANGEROUS - string_view Viewing Temporary Return Value
 * 
 * THE PROBLEM:
 * - Function returns std::string by value (creates temporary)
 * - Temporary is destroyed at end of statement
 * - string_view tries to view this destroyed temporary
 * - Result: Dangling reference
 * 
 * TEMPORARY LIFETIME:
 * Temporaries are destroyed at the end of the full expression (statement)
 * 
 * RULE:
 * Don't initialize string_view with function that returns std::string by value!
 */

#include <iostream>
#include <string>
#include <string_view>

// Function returns std::string BY VALUE
// This creates a TEMPORARY std::string object
std::string getName()
{
    std::string s { "Rama" };
    return s;  // Returns copy (or moved) string
}              // Local 's' destroyed here, but return value is a temporary

int main()
{
  // DANGEROUS CODE:
  // 1. getName() returns temporary std::string
  // 2. name is initialized to view this temporary
  // 3. Temporary is destroyed at end of this statement (after semicolon)
  // 4. name now views deallocated memory!
  std::string_view name { getName() };
  
  // UNDEFINED BEHAVIOR!
  // The temporary returned by getName() is already destroyed
  // name is a dangling reference
  std::cout << name << '\n';  // ☠️ DANGER: Might crash, print garbage, or "work"
  
  // CORRECT APPROACHES:
  // Option 1: Store in std::string (owns the data)
  // std::string name_str { getName() };  // ✓ Safe
  
  // Option 2: Return string_view from function viewing long-lived data
  // (See next examples)

  return 0;
}