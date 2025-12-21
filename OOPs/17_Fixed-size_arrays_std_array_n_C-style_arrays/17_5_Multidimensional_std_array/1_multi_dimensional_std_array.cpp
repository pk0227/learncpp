/*
 * Demonstration: Multidimensional std::array (Nested Array Approach)
 * 
 * Implementation: Array of arrays - std::array<std::array<T, Col>, Row>
 * 
 * Key Concepts:
 * 1. Type alias for cleaner syntax
 * 2. View class provides metadata (rows, cols, length)
 * 3. Uses std::reference_wrapper to avoid copying
 * 4. Natural arr[row][col] syntax
 * 5. Each row is a contiguous std::array
 * 
 * Pros: Intuitive syntax, each row is independently accessible
 * Cons: May not be contiguous in memory (implementation-dependent)
 */

#include <iostream>
#include <array>
#include <functional>

// Type alias for 2D array (array of arrays)
template <typename T, std::size_t Row, std::size_t Col>
using Array2D = std::array<std::array<T, Col>, Row>;

// View wrapper class providing helper methods and clean interface
template <typename T, std::size_t Row, std::size_t Col>
class Array2DView
{
    std::reference_wrapper<Array2D<T, Row, Col>> m_arr_ref{};  // Avoid copying array
public:
    // Constructor: Takes reference to array
    Array2DView(Array2D<T, Row, Col>& ref) : m_arr_ref{ref}
    { }

    // Metadata accessors
    int rows() const { return static_cast<int>(Row); }
    int cols() const { return static_cast<int>(Col); }
    int length() const { return static_cast<int>(Row * Col); }  // Total elements

    // Subscript operators: arr[row] returns reference to entire row
    std::array<T, Col>& operator[](int i) { return m_arr_ref.get()[static_cast<std::size_t>(i)]; }
    const std::array<T, Col>& operator[](int i) const { return m_arr_ref.get()[static_cast<std::size_t>(i)]; }
};



int main()
{
    // Create 3x4 2D array with brace-elision initialization
    // Outer braces for std::array, inner braces elided
    Array2D<int, 3, 4> arr {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12 };

    // Create view wrapper (holds reference, doesn't copy)
    Array2DView<int, 3, 4> arrView { arr };

    // Display array metadata
    std::cout << "Rows: " << arrView.rows() << '\n';
    std::cout << "Cols: " << arrView.cols() << '\n';
    std::cout << "Length: " << arrView.length() << '\n';
    
    std::cout << "=====================================\n";

    // Approach 1: Linear iteration (treating 2D array as 1D)
    // Calculate row/col from linear index
    for (int i=0; i < arrView.length(); ++i)
    {
        int row{i/arrView.cols()};  // Integer division gives row index
        int col{i % arrView.cols()};  // Modulo gives column index
        
        std::cout << arrView[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
        
        // Formatting: space between elements, newline at end of row
        if((i+1) % arrView.cols())
            std::cout << " ";
        else
            std::cout << "\n";
    }

    std::cout << '\n';

    // Approach 2: Natural 2D iteration (nested loops)
    // More intuitive for 2D arrays
    for (int row=0; row < arrView.rows(); ++row)
    {
        for (int col=0; col < arrView.cols(); ++col)
            std::cout << arrView[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)] << ' ';
        std::cout << '\n';
    }

    std::cout << '\n';
    
    return 0;
}