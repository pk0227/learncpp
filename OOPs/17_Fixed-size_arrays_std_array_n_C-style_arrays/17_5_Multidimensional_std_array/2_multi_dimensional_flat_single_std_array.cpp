/*
 * Demonstration: Multidimensional std::array (Flattened Single Array Approach)
 * 
 * Implementation: Single flat array with 2D indexing - std::array<T, Row * Col>
 * 
 * CORE CONCEPT:
 * Instead of std::array<std::array<T, 4>, 3>, we use std::array<T, 12>
 * A 3x4 2D array is stored as a single linear array with 12 elements
 * 
 * ROW-MAJOR LAYOUT (how 2D is mapped to 1D):
 * Logical 2D view:        Physical 1D storage:
 * [0,0] [0,1] [0,2] [0,3]    [0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11]
 * [1,0] [1,1] [1,2] [1,3]     └─row 0──┘ └─row 1──┘ └──row 2───┘
 * [2,0] [2,1] [2,2] [2,3]
 * 
 * INDEXING FORMULA: index = (row * num_cols) + col
 * Example: Element at [1,2] = (1 * 4) + 2 = index 6
 * 
 * Key Concepts:
 * 1. Memory-efficient: Guaranteed contiguous storage
 * 2. Cache-friendly: All elements are adjacent in memory
 * 3. Multiple access methods provided for flexibility:
 *    - operator()(row, col) - function call operator
 *    - operator[](row, col) - C++23 multidimensional subscript
 *    - operator[](row)[col] - traditional chained subscript via proxy
 * 4. Proxy pattern enables arr[row][col] syntax
 * 5. Row-major layout: rows are stored consecutively
 * 
 * Comparison with nested arrays:
 * Pros: Contiguous memory, cache-friendly, single allocation
 * Cons: More complex indexing, requires proxy for arr[row][col] syntax
 */

#include <iostream>
#include <array>
#include <functional>

// Proxy class to enable arr[row][col] syntax with flat array
// Returns reference to a row, which can then be indexed by column
// 
// HOW IT WORKS:
// When you write arr[row][col]:
// 1. arr[row] returns a ProxyRow object (temporary)
// 2. ProxyRow holds a pointer to the start of that row
// 3. [col] calls ProxyRow::operator[](col) to get the element
template <typename T>
class ProxyRow
{
    T* m_row_ptr{};  // Pointer to start of row in flat array
    public:
        // Constructor: Takes pointer to row start
        // 
        // PARAMETER EXPLANATION: const T*&& ref_to_ptr
        // - T*: pointer to element of type T
        // - const: the pointer value itself is const (not what it points to)
        // - &&: rvalue reference (binds to temporary expressions)
        // 
        // WHY &&?
        // The expression "arr.data() + offset" creates a TEMPORARY pointer (rvalue)
        // Using && allows binding to this temporary without copying
        // It's an optimization to accept temporaries directly
        //
        // WHY const_cast?
        // arr.data() might return const T* (for const arrays)
        // But we store T* to allow modification through non-const operator[]
        // const_cast removes the const qualifier from the pointer
        // (Note: This is safe only when the original array is non-const)
        ProxyRow(const T*&& ref_to_ptr) : m_row_ptr{const_cast<T*>(ref_to_ptr)}
        { }

        // Column indexing: proxy[col] returns element
        // This is the second [] in arr[row][col]
        // Uses pointer arithmetic: m_row_ptr[c] is equivalent to *(m_row_ptr + c)
        T& operator[](int c) { return m_row_ptr[c]; }
        const T& operator[](int c) const { return m_row_ptr[c]; }
        
        // Helper for stream output (prints first element of row)
        T getCurrentValue() const { return *m_row_ptr; }
};

// Type alias for flattened 2D array
template <typename T, std::size_t Row, std::size_t Col>
using ArrayFlat2D = std::array<T, Row * Col>;

// View wrapper providing multiple access methods for flat 2D array
template <typename T, std::size_t Row, std::size_t Col>
class ArrayFlat2DView
{
    std::reference_wrapper<ArrayFlat2D<T, Row, Col>> m_arr_ref{};  // Avoid copying
public:
    ArrayFlat2DView(ArrayFlat2D<T, Row, Col>& ref) : m_arr_ref{ref}
    { }

    // Metadata accessors
    int rows() const { return static_cast<int>(Row); }
    int cols() const { return static_cast<int>(Col); }
    int length() const { return static_cast<int>(Row * Col); }

    // Linear indexing (commented out - not used in this demo)
    //T& operator[](int i) { return m_arr_ref.get()[static_cast<std::size_t>(i)]; }
    //const T& operator[](int i) const { return m_arr_ref.get()[static_cast<std::size_t>(i)]; }

    // Method 1: Function call operator - arr(row, col)
    // Traditional approach, works in all C++ versions
    //
    // INDEXING FORMULA: (r * cols()) + c
    // Example for 3x4 array (3 rows, 4 columns):
    //   Element at [1][2] (row 1, col 2):
    //   Index = (1 * 4) + 2 = 6
    //
    // Visual representation of flat array indices:
    // [0][0]=0  [0][1]=1  [0][2]=2  [0][3]=3
    // [1][0]=4  [1][1]=5  [1][2]=6  [1][3]=7
    // [2][0]=8  [2][1]=9  [2][2]=10 [2][3]=11
    T& operator()(int r, int c)
    {
        return m_arr_ref.get()[static_cast<std::size_t>((r * cols())+c)];
    }

    const T& operator()(int r, int c) const
    {
        return m_arr_ref.get()[static_cast<std::size_t>((r * cols())+c)];
    }

    // Method 2: C++23 multidimensional subscript - arr[row, col]
    // New in C++23 (P2128R6): operator[] can take multiple parameters
    // Uses same indexing formula as operator()
#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
    T& operator[](int r, int c)
    {
        return m_arr_ref.get()[static_cast<std::size_t>((r * cols())+c)];
    }

    const T& operator[](int r, int c) const
    {
        return m_arr_ref.get()[static_cast<std::size_t>((r * cols())+c)];
    }
#endif

    // Method 3: Proxy pattern for chained subscript - arr[row][col]
    // Returns ProxyRow which can be indexed again
    // Works in all C++ versions, familiar syntax
    //
    // STEP-BY-STEP BREAKDOWN:
    // 1. m_arr_ref.get() - Get the underlying std::array from reference_wrapper
    // 2. .data() - Get raw pointer to first element of array
    // 3. (r * cols()) - Calculate offset to start of row r
    // 4. pointer + offset - Pointer arithmetic to get address of row start
    // 5. ProxyRow<T>{...} - Create temporary ProxyRow with that address
    //
    // Example: arr[1] in a 3x4 array:
    //   r = 1, cols() = 4
    //   offset = 1 * 4 = 4
    //   Returns pointer to element at index 4 (start of row 1)
    //   ProxyRow stores this pointer
    //   Then arr[1][2] calls ProxyRow::operator[](2)
    //   Which returns *(pointer + 2) = element at index 6
    ProxyRow<T> operator[](int r)
    {
        return ProxyRow<T>{m_arr_ref.get().data() + (r * cols())};  // Pointer to row start
    }

    const ProxyRow<T> operator[](int r) const
    {
        return ProxyRow<T>{m_arr_ref.get().data() + (r * cols())};
    }
};

// Stream operator for ProxyRow (prints first element)
template <typename T>
std::ostream& operator<<(std::ostream& out, const ProxyRow<T>& ref)
{
    out << ref.getCurrentValue();
    return out;
}

int main()
{
    // Create flat 3x4 array (single std::array with 12 elements)
    // Memory layout: [1,2,3,4,5,6,7,8,9,10,11,12]
    ArrayFlat2D<int, 3, 4> arr {
        1, 2, 3, 4,     // Row 0: indices 0-3
        5, 6, 7, 8,     // Row 1: indices 4-7
        9, 10, 11, 12   // Row 2: indices 8-11
    };

    // Create view wrapper
    ArrayFlat2DView<int, 3, 4> arrView { arr };

    // Display metadata
    std::cout << "Rows: " << arrView.rows() << '\n';
    std::cout << "Cols: " << arrView.cols() << '\n';
    std::cout << "Length: " << arrView.length() << '\n';

    // Linear iteration (commented out - not demonstrated here)
    //for (int i=0; i < arrView.length(); ++i)
    //    std::cout << arrView[i] << ' ';

    std::cout << "\n==============================================\n";

    // Access Method 1: Function call operator - arrView(row, col)
    // Works in all C++ versions
    for (int row=0; row < arrView.rows(); ++row)
    {
        for (int col=0; col < arrView.cols(); ++col)
            std::cout << arrView(row, col) << ' ';
        std::cout << '\n';
    }

    std::cout << "\n==============================================\n";

    // Access Method 2: C++23 multidimensional subscript - arrView[row, col]
    // Requires C++23 or later with multidimensional subscript support
#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
    for (int row=0; row < arrView.rows(); ++row)
    {
        for (int col=0; col < arrView.cols(); ++col)
            std::cout << arrView[row, col] << ' ';  // Multiple parameters in []
        std::cout << '\n';
    }
#else
    std::cout << "(Note: C++23 multidimensional arrView[row, col] requires C++23 multidimensional subscript support)\n";
#endif

    std::cout << "\n==============================================\n";

    // Access Method 3: Proxy pattern - arrView[row][col]
    // Traditional chained subscript syntax, works in all C++ versions
    //
    // EXECUTION FLOW for arrView[1][2]:
    // Step 1: arrView[1] is called
    //   - Calls ArrayFlat2DView::operator[](1)
    //   - Calculates: arr.data() + (1 * 4) = pointer to index 4
    //   - Returns ProxyRow{pointer_to_index_4} (temporary object)
    //
    // Step 2: [2] is called on the ProxyRow temporary
    //   - Calls ProxyRow::operator[](2)
    //   - Returns: *(pointer_to_index_4 + 2) = element at index 6
    //   - Which is the value 7 (row 1, col 2)
    for (int row=0; row < arrView.rows(); ++row)
    {
        for (int col=0; col < arrView.cols(); ++col)
            std::cout << arrView[row][col] << ' ';  // Chained operator[]
        std::cout << '\n';
    }

    std::cout << '\n';
    
    return 0;
}