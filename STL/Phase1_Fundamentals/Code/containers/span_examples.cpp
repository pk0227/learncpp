/**
 * Non-Owning Contiguous Views: std::span (C++20) & std::string_view (C++17)
 *
 * Demonstrates:
 * 1. std::span as a unified zero-overhead function parameter (replaces const vector<T>&, array, T*, size)
 * 2. Dynamic Extent vs Static Extent spans
 * 3. Zero-copy subspan slicing (first, last, subspan)
 * 4. std::string_view for zero-copy string manipulation
 * 5. Lifetime warning: Dangling view traps
 *
 * Compile:
 *   g++ -std=c++20 -O2 -Wall -Wextra span_examples.cpp -o span_examples
 */

#include <iostream>
#include <span>
#include <string_view>
#include <vector>
#include <array>
#include <numeric>

// ============================================================================
// 1. UNIFIED FUNCTION PARAMETER (DYNAMIC EXTENT SPAN)
// ============================================================================

// Before C++20: You needed separate overloads for vector, array, raw pointer + size
// In C++20: std::span<const int> binds to ANY contiguous integer sequence with ZERO overhead
void print_elements(std::span<const int> s, std::string_view label) {
    std::cout << label << " (size=" << s.size() << ", bytes=" << s.size_bytes() << "): ";
    for (int x : s) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}

void demo_span_binding() {
    std::cout << "=== 1. std::span Unified Binding ===\n";

    // A. Binds to std::vector
    std::vector<int> vec = {10, 20, 30};
    print_elements(vec, "std::vector");

    // B. Binds to std::array
    std::array<int, 4> arr = {100, 200, 300, 400};
    print_elements(arr, "std::array");

    // C. Binds to C-style raw array
    int raw_c_array[] = {1, 2, 3, 4, 5};
    print_elements(raw_c_array, "C-style array");

    // D. Binds to pointer + length
    print_elements(std::span(raw_c_array + 1, 3), "Pointer + Length");
    std::cout << "\n";
}

// ============================================================================
// 2. STATIC EXTENT VS DYNAMIC EXTENT
// ============================================================================

void demo_static_vs_dynamic() {
    std::cout << "=== 2. Static vs Dynamic Extents ===\n";

    // Dynamic Extent (default): size is stored inside the span struct (pointer + size = 16 bytes on 64-bit)
    std::span<int> dyn_span;
    std::cout << "sizeof(std::span<int>) [dynamic extent]: " << sizeof(dyn_span) << " bytes\n";

    // Static Extent: size is encoded into the compile-time type (pointer only = 8 bytes on 64-bit!)
    std::array<int, 4> data = {1, 2, 3, 4};
    std::span<int, 4> static_span(data);
    std::cout << "sizeof(std::span<int, 4>) [static extent]: " << sizeof(static_span) << " bytes\n\n";
}

// ============================================================================
// 3. ZERO-COPY SUBSPAN SLICING
// ============================================================================

void demo_subspan_slicing() {
    std::cout << "=== 3. Zero-Copy Subspan Slicing ===\n";

    std::vector<int> buffer = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
    std::span<const int> whole(buffer);

    // subspan(offset, count)
    auto middle = whole.subspan(3, 4); // elements at indices 3, 4, 5, 6
    print_elements(middle, "Subspan [3..6]");

    // first(N) and last(N)
    auto prefix = whole.first(3);
    auto suffix = whole.last(3);
    print_elements(prefix, "First 3 elements");
    print_elements(suffix, "Last 3 elements");
    std::cout << "\n";
}

// ============================================================================
// 4. STD::STRING_VIEW (C++17)
// ============================================================================

void process_string_view(std::string_view sv) {
    std::cout << "Processing string_view: \"" << sv << "\" (length=" << sv.length() << ")\n";
    if (sv.starts_with("prefix:")) {
        std::string_view payload = sv.substr(7);
        std::cout << "  Payload extracted without copying: \"" << payload << "\"\n";
    }
}

void demo_string_view() {
    std::cout << "=== 4. std::string_view Zero-Copy Slicing ===\n";

    std::string full_string = "prefix:HelloWorldDataPacket";
    // Passing string without allocation
    process_string_view(full_string);

    // Passing string literal without allocation
    process_string_view("prefix:CommandExecute");
    std::cout << "\n";
}

// ============================================================================
// 5. LIFETIME DANGERS: DANGLING VIEWS
// ============================================================================

/**
 * WARNING: std::span and std::string_view do NOT own the underlying memory!
 * Returning a span to a local variable or temporary rvalue causes a dangling reference.
 */
std::string_view dangerous_dangling_string() {
    std::string local = "Temporary string";
    return local; // ⚠️ DANGER: 'local' is destroyed at function return, returned view dangles!
}

void demo_lifetime_warning() {
    std::cout << "=== 5. Lifetime Safety Rules ===\n";
    std::cout << "Rule 1: Use span/string_view as function parameters.\n";
    std::cout << "Rule 2: Never return span/string_view pointing to local or temporary objects.\n";
    std::cout << "Rule 3: Beware of container reallocations while holding a span into it!\n\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    demo_span_binding();
    demo_static_vs_dynamic();
    demo_subspan_slicing();
    demo_string_view();
    demo_lifetime_warning();

    std::cout << "All span and string_view demonstrations completed successfully.\n";
    return 0;
}
