#include <iostream>
#include <string>
#include <cassert>
#include "GenericArray.hpp"

// ============================================
// Test Class for User-Defined Types
// ============================================
class TestClass
{
    int m_id;
    std::string m_data;
    
public:
    TestClass() : m_id{0}, m_data{"empty"} {}
    TestClass(int id, std::string data) : m_id{id}, m_data{data} {}
    
    int getId() const { return m_id; }
    std::string getData() const { return m_data; }
    
    friend std::ostream& operator<<(std::ostream& out, const TestClass& obj)
    {
        return out << "TestClass(" << obj.m_id << "," << obj.m_data << ")";
    }
};

// ============================================
// Helper Functions
// ============================================
template <typename T>
void printArray(const GenericArray<T>& arr, const std::string& label = "Array")
{
    std::cout << label << " [len=" << arr.getLength() << "]: ";
    for(int i = 0; i < arr.getLength(); ++i)
    {
        std::cout << arr[i];
        if(i < arr.getLength() - 1) std::cout << ", ";
    }
    std::cout << "\n";
}

void separator() { std::cout << "-------------------------------------------\n"; }
void testHeader(const std::string& name) { std::cout << "\n*** " << name << " ***\n"; }

// ============================================
// TEST 1: Default Constructor
// ============================================
void test_default_constructor()
{
    testHeader("TEST 1: Default Constructor");
    
    GenericArray<int> arr;
    std::cout << "Created empty array: length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 0);
    
    std::cout << "PASS: Default constructor works\n";
    separator();
}

// ============================================
// TEST 2: Constructor with Size
// ============================================
void test_size_constructor()
{
    testHeader("TEST 2: Constructor with Size");
    
    // Positive size
    GenericArray<int> arr1(5);
    std::cout << "Created array with size 5: length = " << arr1.getLength() << "\n";
    printArray(arr1, "Elements (default initialized)");
    assert(arr1.getLength() == 5);
    for(int i = 0; i < 5; ++i) assert(arr1[i] == 0);
    
    // Zero size
    GenericArray<int> arr2(0);
    std::cout << "Created array with size 0: length = " << arr2.getLength() << "\n";
    assert(arr2.getLength() == 0);
    
    // Negative size (should default to 0)
    GenericArray<int> arr3(-10);
    std::cout << "Created array with size -10: length = " << arr3.getLength() << "\n";
    assert(arr3.getLength() == 0);
    
    std::cout << "PASS: Size constructor works correctly\n";
    separator();
}

// ============================================
// TEST 3: Initializer List Constructor
// ============================================
void test_initializer_list_constructor()
{
    testHeader("TEST 3: Initializer List Constructor");
    
    GenericArray<int> arr{10, 20, 30, 40, 50};
    printArray(arr, "Array created with {10,20,30,40,50}");
    assert(arr.getLength() == 5);
    assert(arr[0] == 10 && arr[4] == 50);
    
    GenericArray<int> empty{};
    std::cout << "Empty initializer list: length = " << empty.getLength() << "\n";
    assert(empty.getLength() == 0);
    
    std::cout << "PASS: Initializer list constructor works\n";
    separator();
}

// ============================================
// TEST 4: Copy Constructor
// ============================================
void test_copy_constructor()
{
    testHeader("TEST 4: Copy Constructor");
    
    GenericArray<int> original{100, 200, 300};
    printArray(original, "Original");
    
    GenericArray<int> copy(original);
    printArray(copy, "Copy");
    
    assert(copy.getLength() == original.getLength());
    for(int i = 0; i < copy.getLength(); ++i)
        assert(copy[i] == original[i]);
    
    // Verify deep copy
    copy[0] = 999;
    std::cout << "Modified copy[0] = 999\n";
    std::cout << "Original[0] = " << original[0] << " (should remain 100)\n";
    std::cout << "Copy[0] = " << copy[0] << " (should be 999)\n";
    assert(original[0] == 100 && copy[0] == 999);
    
    std::cout << "PASS: Copy constructor creates deep copy\n";
    separator();
}

// ============================================
// TEST 5: Copy Assignment Operator
// ============================================
void test_copy_assignment()
{
    testHeader("TEST 5: Copy Assignment Operator");
    
    GenericArray<int> arr1{1, 2, 3};
    GenericArray<int> arr2;
    
    printArray(arr1, "arr1 before assignment");
    std::cout << "arr2 before assignment: length = " << arr2.getLength() << "\n";
    
    arr2 = arr1;
    printArray(arr2, "arr2 after assignment");
    
    assert(arr2.getLength() == 3);
    assert(arr2[0] == 1 && arr2[2] == 3);
    
    // Verify deep copy
    arr2[1] = 999;
    std::cout << "Modified arr2[1] = 999\n";
    std::cout << "arr1[1] = " << arr1[1] << " (should remain 2)\n";
    assert(arr1[1] == 2);
    
    std::cout << "PASS: Copy assignment works correctly\n";
    separator();
}

// ============================================
// TEST 6: Self Assignment
// ============================================
void test_self_assignment()
{
    testHeader("TEST 6: Self Assignment");
    
    GenericArray<int> arr{5, 10, 15, 20};
    printArray(arr, "Before self-assignment");
    
    arr = arr;  // Self-assignment
    printArray(arr, "After self-assignment");
    
    assert(arr.getLength() == 4);
    assert(arr[0] == 5 && arr[3] == 20);
    
    std::cout << "PASS: Self-assignment handled correctly\n";
    separator();
}

// ============================================
// TEST 7: Initializer List Assignment
// ============================================
void test_initializer_list_assignment()
{
    testHeader("TEST 7: Initializer List Assignment");
    
    GenericArray<int> arr{1, 2};
    printArray(arr, "Initial array");
    
    arr = {10, 20, 30, 40, 50};
    printArray(arr, "After assignment {10,20,30,40,50}");
    assert(arr.getLength() == 5 && arr[4] == 50);
    
    arr = {100};
    printArray(arr, "After assignment {100}");
    assert(arr.getLength() == 1 && arr[0] == 100);
    
    arr = {};
    std::cout << "After assignment {}: length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 0);
    
    std::cout << "PASS: Initializer list assignment works\n";
    separator();
}

// ============================================
// TEST 8: Element Access operator[]
// ============================================
void test_element_access()
{
    testHeader("TEST 8: Element Access operator[]");
    
    GenericArray<int> arr(5);
    
    // Write access
    for(int i = 0; i < 5; ++i)
        arr[i] = (i + 1) * 10;
    
    printArray(arr, "After writing values");
    
    // Read access
    std::cout << "Reading elements: ";
    for(int i = 0; i < 5; ++i)
    {
        std::cout << "arr[" << i << "]=" << arr[i] << " ";
        assert(arr[i] == (i + 1) * 10);
    }
    std::cout << "\n";
    
    // Const access
    const GenericArray<int> constArr{7, 8, 9};
    std::cout << "Const array: constArr[0]=" << constArr[0] << ", constArr[2]=" << constArr[2] << "\n";
    assert(constArr[0] == 7 && constArr[2] == 9);
    
    std::cout << "PASS: Element access works correctly\n";
    separator();
}

// ============================================
// TEST 9: erase() Function
// ============================================
void test_erase()
{
    testHeader("TEST 9: erase() Function");
    
    GenericArray<int> arr{1, 2, 3, 4, 5};
    printArray(arr, "Before erase");
    
    arr.erase();
    std::cout << "After erase(): length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 0);
    
    // Erase again (should be safe)
    arr.erase();
    std::cout << "After second erase(): length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 0);
    
    std::cout << "PASS: erase() works correctly\n";
    separator();
}

// ============================================
// TEST 10: reallocate() Function
// ============================================
void test_reallocate()
{
    testHeader("TEST 10: reallocate() Function");
    
    GenericArray<int> arr{10, 20, 30};
    printArray(arr, "Original array");
    
    arr.reallocate(7);
    std::cout << "After reallocate(7): length = " << arr.getLength() << "\n";
    printArray(arr, "Elements (all should be 0)");
    assert(arr.getLength() == 7);
    for(int i = 0; i < 7; ++i) assert(arr[i] == 0);
    
    arr.reallocate(0);
    std::cout << "After reallocate(0): length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 0);
    
    arr.reallocate(-5);
    std::cout << "After reallocate(-5): length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 0);
    
    std::cout << "PASS: reallocate() works correctly\n";
    separator();
}

// ============================================
// TEST 11: resize() - Grow Array
// ============================================
void test_resize_grow()
{
    testHeader("TEST 11: resize() - Grow Array");
    
    GenericArray<int> arr{1, 2, 3};
    printArray(arr, "Original");
    
    arr.resize(6);
    printArray(arr, "After resize(6)");
    
    assert(arr.getLength() == 6);
    assert(arr[0] == 1 && arr[1] == 2 && arr[2] == 3);
    std::cout << "New elements: arr[3]=" << arr[3] << ", arr[4]=" << arr[4] << ", arr[5]=" << arr[5] << "\n";
    assert(arr[3] == 0 && arr[4] == 0 && arr[5] == 0);
    
    std::cout << "PASS: resize() grow works correctly\n";
    separator();
}

// ============================================
// TEST 12: resize() - Shrink Array
// ============================================
void test_resize_shrink()
{
    testHeader("TEST 12: resize() - Shrink Array");
    
    GenericArray<int> arr{10, 20, 30, 40, 50};
    printArray(arr, "Original");
    
    arr.resize(3);
    printArray(arr, "After resize(3)");
    
    assert(arr.getLength() == 3);
    assert(arr[0] == 10 && arr[1] == 20 && arr[2] == 30);
    
    std::cout << "PASS: resize() shrink works correctly\n";
    separator();
}

// ============================================
// TEST 13: resize() - Edge Cases
// ============================================
void test_resize_edge_cases()
{
    testHeader("TEST 13: resize() - Edge Cases");
    
    GenericArray<int> arr{1, 2, 3};
    
    // Resize to same size (should be no-op)
    arr.resize(3);
    printArray(arr, "After resize(3) - same size");
    assert(arr.getLength() == 3);
    
    // Resize to 0
    arr.resize(0);
    std::cout << "After resize(0): length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 0);
    
    // Resize negative (should call erase)
    arr = {5, 6, 7};
    arr.resize(-10);
    std::cout << "After resize(-10): length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 0);
    
    std::cout << "PASS: resize() edge cases work correctly\n";
    separator();
}

// ============================================
// TEST 14: insert() - Valid Positions
// ============================================
void test_insert_valid()
{
    testHeader("TEST 14: insert() - Valid Positions");
    
    GenericArray<int> arr{10, 20, 30};
    printArray(arr, "Original");
    
    // Insert at beginning
    bool result = arr.insert(0, 5);
    std::cout << "insert(0, 5): " << (result ? "Success" : "Failed") << "\n";
    printArray(arr, "After insert at beginning");
    assert(result && arr.getLength() == 4 && arr[0] == 5 && arr[1] == 10);
    
    // Insert in middle
    result = arr.insert(2, 15);
    std::cout << "insert(2, 15): " << (result ? "Success" : "Failed") << "\n";
    printArray(arr, "After insert in middle");
    assert(result && arr.getLength() == 5 && arr[2] == 15);
    
    // Insert at last valid index
    result = arr.insert(arr.getLength() - 1, 99);
    std::cout << "insert(" << (arr.getLength()-1) << ", 99): " << (result ? "Success" : "Failed") << "\n";
    printArray(arr, "After insert at last valid index");
    assert(result);
    
    std::cout << "PASS: insert() at valid positions works\n";
    separator();
}

// ============================================
// TEST 15: insert() - Invalid Positions
// ============================================
void test_insert_invalid()
{
    testHeader("TEST 15: insert() - Invalid Positions");
    
    GenericArray<int> arr{10, 20, 30};
    printArray(arr, "Original");
    int originalLen = arr.getLength();
    
    // Negative index
    bool result = arr.insert(-1, 999);
    std::cout << "insert(-1, 999): " << (result ? "Success" : "Failed") << " (should fail)\n";
    assert(!result && arr.getLength() == originalLen);
    
    // Index == length (out of bounds per your design)
    result = arr.insert(arr.getLength(), 999);
    std::cout << "insert(" << arr.getLength() << ", 999): " << (result ? "Success" : "Failed") << " (should fail)\n";
    assert(!result && arr.getLength() == originalLen);
    
    // Index > length
    result = arr.insert(100, 999);
    std::cout << "insert(100, 999): " << (result ? "Success" : "Failed") << " (should fail)\n";
    assert(!result && arr.getLength() == originalLen);
    
    printArray(arr, "Array unchanged");
    
    // Insert on empty array
    GenericArray<int> empty;
    result = empty.insert(0, 42);
    std::cout << "insert(0, 42) on empty array: " << (result ? "Success" : "Failed") << " (should fail)\n";
    assert(!result && empty.getLength() == 0);
    
    std::cout << "PASS: insert() correctly rejects invalid positions\n";
    separator();
}

// ============================================
// TEST 16: remove() - Valid Positions
// ============================================
void test_remove_valid()
{
    testHeader("TEST 16: remove() - Valid Positions");
    
    GenericArray<int> arr{10, 20, 30, 40, 50};
    printArray(arr, "Original");
    
    // Remove from middle
    bool result = arr.remove(2);
    std::cout << "remove(2): " << (result ? "Success" : "Failed") << "\n";
    printArray(arr, "After remove from middle");
    assert(result && arr.getLength() == 4 && arr[2] == 40);
    
    // Remove from beginning
    result = arr.remove(0);
    std::cout << "remove(0): " << (result ? "Success" : "Failed") << "\n";
    printArray(arr, "After remove from beginning");
    assert(result && arr.getLength() == 3 && arr[0] == 20);
    
    // Remove from end
    result = arr.remove(arr.getLength() - 1);
    std::cout << "remove(" << arr.getLength() << "): " << (result ? "Success" : "Failed") << "\n";
    printArray(arr, "After remove from end");
    assert(result && arr.getLength() == 2);
    
    std::cout << "PASS: remove() at valid positions works\n";
    separator();
}

// ============================================
// TEST 17: remove() - Single Element
// ============================================
void test_remove_single_element()
{
    testHeader("TEST 17: remove() - Single Element");
    
    GenericArray<int> arr{42};
    printArray(arr, "Array with single element");
    
    bool result = arr.remove(0);
    std::cout << "remove(0): " << (result ? "Success" : "Failed") << "\n";
    std::cout << "After removal: length = " << arr.getLength() << "\n";
    
    assert(result && arr.getLength() == 0);
    
    std::cout << "PASS: remove() single element works correctly\n";
    separator();
}

// ============================================
// TEST 18: remove() - Invalid Positions
// ============================================
void test_remove_invalid()
{
    testHeader("TEST 18: remove() - Invalid Positions");
    
    GenericArray<int> arr{10, 20, 30};
    printArray(arr, "Original");
    int originalLen = arr.getLength();
    
    // Negative index
    bool result = arr.remove(-1);
    std::cout << "remove(-1): " << (result ? "Success" : "Failed") << " (should fail)\n";
    assert(!result && arr.getLength() == originalLen);
    
    // Index == length
    result = arr.remove(arr.getLength());
    std::cout << "remove(" << arr.getLength() << "): " << (result ? "Success" : "Failed") << " (should fail)\n";
    assert(!result && arr.getLength() == originalLen);
    
    // Index > length
    result = arr.remove(100);
    std::cout << "remove(100): " << (result ? "Success" : "Failed") << " (should fail)\n";
    assert(!result && arr.getLength() == originalLen);
    
    printArray(arr, "Array unchanged");
    
    // Remove from empty array
    GenericArray<int> empty;
    result = empty.remove(0);
    std::cout << "remove(0) on empty array: " << (result ? "Success" : "Failed") << " (should fail)\n";
    assert(!result && empty.getLength() == 0);
    
    std::cout << "PASS: remove() correctly rejects invalid positions\n";
    separator();
}

// ============================================
// TEST 19: Type - double
// ============================================
void test_type_double()
{
    testHeader("TEST 19: Type - double");
    
    GenericArray<double> arr{1.1, 2.2, 3.3, 4.4, 5.5};
    printArray(arr, "Double array");
    
    arr.insert(2, 2.75);
    printArray(arr, "After insert(2, 2.75)");
    assert(arr.getLength() == 6 && arr[2] == 2.75);
    
    arr.remove(0);
    printArray(arr, "After remove(0)");
    assert(arr.getLength() == 5 && arr[0] == 2.2);
    
    arr.resize(8);
    std::cout << "After resize(8): length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 8);
    
    std::cout << "PASS: double type works correctly\n";
    separator();
}

// ============================================
// TEST 20: Type - char
// ============================================
void test_type_char()
{
    testHeader("TEST 20: Type - char");
    
    GenericArray<char> arr{'A', 'B', 'C', 'D'};
    printArray(arr, "Char array");
    
    arr.insert(1, 'X');
    printArray(arr, "After insert(1, 'X')");
    assert(arr.getLength() == 5 && arr[1] == 'X');
    
    arr.remove(0);
    printArray(arr, "After remove(0)");
    assert(arr.getLength() == 4 && arr[0] == 'X');
    
    std::cout << "PASS: char type works correctly\n";
    separator();
}

// ============================================
// TEST 21: Type - std::string
// ============================================
void test_type_string()
{
    testHeader("TEST 21: Type - std::string");
    
    GenericArray<std::string> arr{"Hello", "World", "C++"};
    printArray(arr, "String array");
    
    arr.insert(1, "Beautiful");
    printArray(arr, "After insert(1, \"Beautiful\")");
    assert(arr.getLength() == 4 && arr[1] == "Beautiful");
    
    arr.remove(2);
    printArray(arr, "After remove(2)");
    assert(arr.getLength() == 3 && arr[2] == "C++");
    
    GenericArray<std::string> copy(arr);
    printArray(copy, "Copied string array");
    assert(copy.getLength() == 3 && copy[0] == "Hello");
    
    std::cout << "PASS: std::string type works correctly\n";
    separator();
}

// ============================================
// TEST 22: Type - bool
// ============================================
void test_type_bool()
{
    testHeader("TEST 22: Type - bool");
    
    GenericArray<bool> arr{true, false, true};
    printArray(arr, "Bool array");
    
    arr.insert(1, false);
    printArray(arr, "After insert(1, false)");
    assert(arr.getLength() == 4);
    
    arr.resize(6);
    std::cout << "After resize(6): length = " << arr.getLength() << "\n";
    assert(arr.getLength() == 6);
    
    std::cout << "PASS: bool type works correctly\n";
    separator();
}

// ============================================
// TEST 23: Type - User-Defined Class
// ============================================
void test_type_user_defined()
{
    testHeader("TEST 23: Type - User-Defined Class");
    
    GenericArray<TestClass> arr(3);
    std::cout << "Created array with 3 TestClass objects\n";
    printArray(arr, "Default constructed");
    
    arr[0] = TestClass(1, "first");
    arr[1] = TestClass(2, "second");
    arr[2] = TestClass(3, "third");
    printArray(arr, "After assignment");
    
    arr.insert(1, TestClass(99, "inserted"));
    printArray(arr, "After insert");
    assert(arr.getLength() == 4 && arr[1].getId() == 99);
    
    GenericArray<TestClass> copy(arr);
    printArray(copy, "Copied array");
    assert(copy.getLength() == 4);
    
    arr.remove(0);
    printArray(arr, "After remove(0)");
    assert(arr.getLength() == 3);
    
    std::cout << "PASS: User-defined class works correctly\n";
    separator();
}

// ============================================
// TEST 24: Large Array
// ============================================
void test_large_array()
{
    testHeader("TEST 24: Large Array (1000 elements)");
    
    GenericArray<int> large(1000);
    std::cout << "Created array with 1000 elements\n";
    
    for(int i = 0; i < 1000; ++i)
        large[i] = i * 2;
    
    std::cout << "First 5: " << large[0] << ", " << large[1] << ", " << large[2] << ", " << large[3] << ", " << large[4] << "\n";
    std::cout << "Last 5: " << large[995] << ", " << large[996] << ", " << large[997] << ", " << large[998] << ", " << large[999] << "\n";
    
    assert(large[0] == 0 && large[500] == 1000 && large[999] == 1998);
    
    GenericArray<int> copy = large;
    std::cout << "Copied large array: length = " << copy.getLength() << "\n";
    assert(copy.getLength() == 1000);
    
    large.resize(10);
    std::cout << "After resize(10): length = " << large.getLength() << "\n";
    assert(large.getLength() == 10);
    
    std::cout << "PASS: Large array works correctly\n";
    separator();
}

// ============================================
// TEST 25: Multiple Operations Sequence
// ============================================
void test_multiple_operations()
{
    testHeader("TEST 25: Multiple Operations Sequence");
    
    GenericArray<int> arr;
    std::cout << "Start with empty array\n";
    
    arr = {1, 2, 3};
    printArray(arr, "After assignment {1,2,3}");
    
    arr.insert(1, 10);
    printArray(arr, "After insert(1, 10)");
    
    arr.resize(6);
    printArray(arr, "After resize(6)");
    
    arr.remove(0);
    printArray(arr, "After remove(0)");
    
    GenericArray<int> copy = arr;
    printArray(copy, "Copied array");
    
    arr.erase();
    std::cout << "After erase(): original length = " << arr.getLength() << "\n";
    printArray(copy, "Copy still intact");
    
    assert(arr.getLength() == 0 && copy.getLength() == 5);
    
    std::cout << "PASS: Multiple operations work correctly\n";
    separator();
}

// ============================================
// TEST 26: Const Correctness
// ============================================
void test_const_correctness()
{
    testHeader("TEST 26: Const Correctness");
    
    const GenericArray<int> constArr{5, 10, 15, 20};
    
    std::cout << "Const array length: " << constArr.getLength() << "\n";
    std::cout << "Const array elements: ";
    for(int i = 0; i < constArr.getLength(); ++i)
        std::cout << constArr[i] << " ";
    std::cout << "\n";
    
    assert(constArr.getLength() == 4 && constArr[0] == 5);
    
    // The following would not compile (as expected):
    // constArr[0] = 99;
    // constArr.erase();
    // constArr.insert(0, 1);
    
    std::cout << "PASS: Const correctness verified\n";
    separator();
}

// ============================================
// MAIN TEST RUNNER
// ============================================
int main()
{
    std::cout << "==================================================\n";
    std::cout << "    COMPLETE GENERICARRAY TEST SUITE\n";
    std::cout << "==================================================\n";
    
    try {
        // Constructor tests
        test_default_constructor();
        test_size_constructor();
        test_initializer_list_constructor();
        test_copy_constructor();
        
        // Assignment tests
        test_copy_assignment();
        test_self_assignment();
        test_initializer_list_assignment();
        
        // Member function tests
        test_element_access();
        test_erase();
        test_reallocate();
        
        // Resize tests
        test_resize_grow();
        test_resize_shrink();
        test_resize_edge_cases();
        
        // Insert tests
        test_insert_valid();
        test_insert_invalid();
        
        // Remove tests
        test_remove_valid();
        test_remove_single_element();
        test_remove_invalid();
        
        // Different data types
        test_type_double();
        test_type_char();
        test_type_string();
        test_type_bool();
        test_type_user_defined();
        
        // Stress tests
        test_large_array();
        test_multiple_operations();
        
        // Const correctness
        test_const_correctness();
        
        std::cout << "\n==================================================\n";
        std::cout << "   ✓✓✓ ALL 26 TESTS PASSED ✓✓✓\n";
        std::cout << "   GenericArray implementation is correct!\n";
        std::cout << "==================================================\n";
        
        return 0;
    }
    catch(const std::exception& e) {
        std::cerr << "\n❌ TEST FAILED: " << e.what() << "\n";
        return 1;
    }
}
