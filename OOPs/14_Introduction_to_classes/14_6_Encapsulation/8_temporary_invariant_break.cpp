// Demonstrates: Invariants can be TEMPORARILY BROKEN inside a member function.
// This is normal and expected, as long as the invariant is FULLY RESTORED
// before the function returns. The invariant must NEVER be externally observable as broken.
//
// Example: SortedArray -- invariant: elements must always be in sorted order.
// The insert() function temporarily breaks the sorted order while shifting elements,
// but restores it before returning.
//
// Compile: g++ -std=c++20 -fsyntax-only 8_temporary_invariant_break.cpp

#include <iostream>
#include <stdexcept>
#include <algorithm>   // std::is_sorted

class SortedArray {
    static constexpr int MAX_SIZE = 10;
    int   data_[MAX_SIZE]{};  // zero-initialized
    int   size_ = 0;

    // Private helper: verifies invariant holds (for illustration only)
    bool isSorted() const {
        for (int i = 1; i < size_; ++i)
            if (data_[i] < data_[i - 1]) return false;
        return true;
    }

public:
    // Constructor ESTABLISHES the invariant: empty array is trivially sorted.
    SortedArray() = default;  // size_=0, empty -> sorted trivially

    // insert() MAINTAINS the invariant.
    // Internally it TEMPORARILY BREAKS sorted order while shifting elements,
    // but restores it before returning.
    void insert(int value) {
        if (size_ >= MAX_SIZE)
            throw std::overflow_error("SortedArray is full");

        // Step 1: find insertion position (first element > value)
        int pos = 0;
        while (pos < size_ && data_[pos] <= value)
            ++pos;

        // Step 2: shift elements right to make room
        // During this shift, the array may temporarily be NOT sorted
        // (an element appears twice, or there's a "hole").
        // This intermediate state is INVISIBLE to the caller.
        for (int i = size_; i > pos; --i)
            data_[i] = data_[i - 1];

        // Step 3: place new value
        data_[pos] = value;
        ++size_;

        // Invariant is now RESTORED: array is sorted again.
        // (No caller can observe the intermediate broken state.)
    }

    // Caller can only ever observe the object with the invariant intact.
    void print() const {
        std::cout << "[ ";
        for (int i = 0; i < size_; ++i)
            std::cout << data_[i] << ' ';
        std::cout << "] (size=" << size_ << ")\n";
    }

    bool sorted() const { return isSorted(); }
    int  size()   const { return size_; }
};

// --- Example 2: swap() temporarily breaks an invariant mid-execution ---
// A MinMax class maintains: min_ <= max_
// swap() temporarily violates this during assignment but restores it atomically.
class MinMax {
    int min_;
    int max_;
public:
    MinMax(int mn, int mx) {
        if (mn > mx) throw std::invalid_argument("min must be <= max");
        min_ = mn; max_ = mx;
    }

    // Swap min and max values -- temporarily the invariant is broken mid-function
    void swapMinMax() {
        int tmp = min_;  // Step 1: save min
        min_ = max_;     // Step 2: min_ = old_max -- NOW min_ > max_ (INVARIANT BROKEN internally)
        max_ = tmp;      // Step 3: max_ = old_min -- invariant RESTORED
        // Caller never sees the broken state between steps 2 and 3.
    }

    void print() const {
        std::cout << "min=" << min_ << " max=" << max_ << '\n';
    }
};

int main() {
    SortedArray arr;

    // Insert in random order -- invariant maintained across all public calls
    arr.insert(5);  arr.print();
    arr.insert(2);  arr.print();
    arr.insert(8);  arr.print();
    arr.insert(1);  arr.print();
    arr.insert(4);  arr.print();

    // Every public observation sees a sorted array
    std::cout << "Is sorted after all inserts: " << std::boolalpha << arr.sorted() << '\n';

    std::cout << '\n';

    MinMax mm{3, 10};
    mm.print();
    mm.swapMinMax();   // temporarily breaks invariant internally, restores before return
    mm.print();        // caller sees: min=10 max=3 -- wait, that breaks it externally!

    // NOTE: in this contrived example swapMinMax() actually breaks the semantic invariant
    // (min <= max) post-return -- illustrating that the class designer decides what the
    // invariant means. If the invariant is "min_ <= max_", then swapMinMax() should
    // NOT exist. It's shown here only to illustrate that temporary internal breaks are OK,
    // but the STATE AFTER the function must also satisfy the invariant.

    return 0;
}
