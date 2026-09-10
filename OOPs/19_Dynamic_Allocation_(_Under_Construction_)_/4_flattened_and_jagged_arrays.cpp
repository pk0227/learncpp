/**
 * @file 4_flattened_and_jagged_arrays.cpp
 * @brief Demonstrates dynamic jagged arrays and 1D flattened 2D arrays
 * 
 * Key Concepts:
 * 1. Array of pointers (int**) allows allocating non-rectangular (jagged/triangular) arrays.
 * 2. Deallocation of jagged arrays must happen in REVERSE order: delete each row first, then delete the row pointer array.
 * 3. Flattening a 2D array of size (rows x cols) into a single 1D array of size (rows * cols) is simpler, faster, and less error-prone.
 * 4. Index mapping formula: index = (row * cols) + col.
 */

#include <iostream>

int main()
{
    // ==========================================
    // Part 1: Jagged / Triangular 2D Array
    // ==========================================
    constexpr int rows{ 4 };
    int** jagged{ new int*[rows] };

    // Allocate rows with increasing length (triangular)
    for (int r{ 0 }; r < rows; ++r)
    {
        jagged[r] = new int[r + 1];
        for (int c{ 0 }; c <= r; ++c)
        {
            jagged[r][c] = (r + 1) * 10 + (c + 1);
        }
    }

    std::cout << "Jagged array output:\n";
    for (int r{ 0 }; r < rows; ++r)
    {
        for (int c{ 0 }; c <= r; ++c)
        {
            std::cout << jagged[r][c] << ' ';
        }
        std::cout << '\n';
    }

    // Cleanup jagged array in reverse order:
    for (int r{ 0 }; r < rows; ++r)
    {
        delete[] jagged[r]; // Delete individual row arrays first
    }
    delete[] jagged;       // Delete array of row pointers last
    jagged = nullptr;

    // ==========================================
    // Part 2: Flattened 1D Array for 2D Grid
    // ==========================================
    constexpr int gridRows{ 3 };
    constexpr int gridCols{ 4 };
    int* flat{ new int[gridRows * gridCols] };

    auto getIndex = [](int r, int c, int cols) {
        return (r * cols) + c;
    };

    // Populate using 2D coordinates mapped to 1D
    for (int r{ 0 }; r < gridRows; ++r)
    {
        for (int c{ 0 }; c < gridCols; ++c)
        {
            flat[getIndex(r, c, gridCols)] = r * 10 + c;
        }
    }

    std::cout << "\nFlattened 2D array output:\n";
    for (int r{ 0 }; r < gridRows; ++r)
    {
        for (int c{ 0 }; c < gridCols; ++c)
        {
            std::cout << flat[getIndex(r, c, gridCols)] << ' ';
        }
        std::cout << '\n';
    }

    // Deallocation is a single delete[]!
    delete[] flat;
    flat = nullptr;

    return 0;
}
