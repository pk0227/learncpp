/**
 * @file 1_spaceship_operator_ordering_categories.cpp
 * @brief Demonstrates C++20 Three-Way Comparison (<=>) and Ordering Categories:
 *        - std::strong_ordering (Total order, substitutability).
 *        - std::weak_ordering (Equivalence without identity, e.g. case-insensitivity).
 *        - std::partial_ordering (Incomparable values, e.g. NaN).
 *        - Compiler-synthesized secondary relational operators.
 */

#include <iostream>
#include <compare>
#include <string>
#include <cctype>
#include <cmath>

// 1. std::strong_ordering: Defaulted member-wise spaceship operator
struct Employee {
    int department_id;
    int employee_id;

    // Synthesizes <=>, ==, !=, <, <=, >, >= with strong ordering!
    auto operator<=>(const Employee&) const = default;
};

// 2. std::weak_ordering: Case-insensitive string comparison
// "Hello" and "hello" are EQUIVALENT, but not IDENTICAL (substitutable)!
class CaseInsensitiveString {
private:
    std::string m_str;

public:
    CaseInsensitiveString(std::string s) : m_str(std::move(s)) {}

    std::weak_ordering operator<=>(const CaseInsensitiveString& other) const {
        std::size_t min_len = std::min(m_str.size(), other.m_str.size());
        for (std::size_t i = 0; i < min_len; ++i) {
            unsigned char c1 = std::tolower(static_cast<unsigned char>(m_str[i]));
            unsigned char c2 = std::tolower(static_cast<unsigned char>(other.m_str[i]));
            if (c1 < c2) return std::weak_ordering::less;
            if (c1 > c2) return std::weak_ordering::greater;
        }
        return m_str.size() <=> other.m_str.size();
    }

    bool operator==(const CaseInsensitiveString& other) const {
        return (*this <=> other) == 0;
    }

    const std::string& get() const { return m_str; }
};

// 3. std::partial_ordering: Floating point value with NaN support
struct FloatValue {
    double val;

    std::partial_ordering operator<=>(const FloatValue& other) const {
        if (std::isnan(val) || std::isnan(other.val)) {
            return std::partial_ordering::unordered; // Cannot be ordered!
        }
        return val <=> other.val;
    }

    bool operator==(const FloatValue& other) const {
        return (*this <=> other) == 0;
    }
};

int main() {
    std::cout << "=== 1. Strong Ordering (Defaulted <=>) ===\n";
    Employee e1{10, 101};
    Employee e2{10, 102};
    std::cout << "e1 < e2:  " << std::boolalpha << (e1 < e2) << "\n";
    std::cout << "e1 == e2: " << (e1 == e2) << "\n\n";

    std::cout << "=== 2. Weak Ordering (Case-Insensitive String) ===\n";
    CaseInsensitiveString s1("Apple");
    CaseInsensitiveString s2("apple");
    auto cmp_weak = (s1 <=> s2);
    if (cmp_weak == std::weak_ordering::equivalent) {
        std::cout << "\"" << s1.get() << "\" and \"" << s2.get() 
                  << "\" are EQUIVALENT (weak ordering)!\n";
    }
    std::cout << "s1 == s2: " << (s1 == s2) << "\n\n";

    std::cout << "=== 3. Partial Ordering (Floating-Point NaN) ===\n";
    FloatValue f_normal{3.14};
    FloatValue f_nan{std::numeric_limits<double>::quiet_NaN()};
    auto cmp_partial = (f_normal <=> f_nan);
    if (cmp_partial == std::partial_ordering::unordered) {
        std::cout << "Comparison with NaN returned UNORDERED!\n";
    }
    std::cout << "f_normal < f_nan:  " << (f_normal < f_nan) << " (false!)\n";
    std::cout << "f_normal > f_nan:  " << (f_normal > f_nan) << " (false!)\n";
    std::cout << "f_normal == f_nan: " << (f_normal == f_nan) << " (false!)\n";

    return 0;
}
