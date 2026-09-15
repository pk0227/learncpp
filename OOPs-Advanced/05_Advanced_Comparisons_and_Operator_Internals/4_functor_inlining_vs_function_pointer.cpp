/**
 * @file 4_functor_inlining_vs_function_pointer.cpp
 * @brief Demonstrates Function Objects (Functors) vs Function Pointers & Multidimensional Indexing:
 *        - Why functors and lambdas are inlined by templates while function pointers are not.
 *        - Functor state retention across calls.
 *        - Multidimensional indexing via operator() and C++23 multidimensional operator[].
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// 1. Raw Function Pointer Comparator
bool functionPointerLess(int a, int b) {
    return a < b;
}

// 2. Functor Comparator (Type is baked into the template!)
struct FunctorLess {
    bool operator()(int a, int b) const noexcept {
        return a < b; // Inlined directly into the algorithm loop!
    }
};

// 3. Stateful Functor (Accumulator)
class RunningAverage {
private:
    double m_sum{0.0};
    std::size_t m_count{0};

public:
    double operator()(double val) {
        m_sum += val;
        ++m_count;
        return m_sum / m_count;
    }
};

// 4. 2D Matrix demonstrating operator() vs C++23 multidimensional operator[]
class Matrix2D {
private:
    std::size_t m_cols;
    std::vector<int> m_data;

public:
    Matrix2D(std::size_t rows, std::size_t cols) 
        : m_cols(cols), m_data(rows * cols, 0) {}

    // Traditional Multidimensional Access via operator()
    int& operator()(std::size_t r, std::size_t c) {
        return m_data[r * m_cols + c];
    }

    // Modern C++23 Multidimensional Subscripting via operator[]
    int& operator[](std::size_t r, std::size_t c) {
        return m_data[r * m_cols + c];
    }
};

int main() {
    std::cout << "=== 1. Inlining Comparison: Functor vs Function Pointer ===\n";
    std::vector<int> data1 = {5, 2, 8, 1, 9};
    std::vector<int> data2 = data1;

    // std::sort with function pointer: Cannot inline! Must invoke via indirect branch!
    std::sort(data1.begin(), data1.end(), functionPointerLess);

    // std::sort with functor: FunctorLess type is instantiated into the template!
    // The compiler inlines operator() into a single 'cmov' or 'cmp' instruction!
    std::sort(data2.begin(), data2.end(), FunctorLess{});
    std::cout << "Data sorted cleanly. Functor version incurs 0 indirect call overhead!\n\n";

    std::cout << "=== 2. Stateful Functor ===\n";
    RunningAverage avg;
    std::cout << "Added 10.0 -> Running average: " << avg(10.0) << "\n";
    std::cout << "Added 20.0 -> Running average: " << avg(20.0) << "\n";
    std::cout << "Added 30.0 -> Running average: " << avg(30.0) << "\n\n";

    std::cout << "=== 3. Multidimensional Indexing ===\n";
    Matrix2D mat(3, 3);
    mat(1, 1) = 42;       // Traditional C++ operator()
    mat[1, 2] = 84;       // Modern C++23 multidimensional operator[]
    std::cout << "mat(1, 1) = " << mat(1, 1) << "\n";
    std::cout << "mat[1, 2] = " << mat[1, 2] << " (Native C++23 syntax!)\n";

    return 0;
}
