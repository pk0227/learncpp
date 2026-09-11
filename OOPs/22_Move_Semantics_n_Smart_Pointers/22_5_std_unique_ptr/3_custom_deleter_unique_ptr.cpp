/**
 * @file 3_custom_deleter_unique_ptr.cpp
 * @brief Demonstrates custom deleters in std::unique_ptr vs std::shared_ptr.
 *
 * Concepts covered:
 * 1. Default deletion via delete / delete[].
 * 2. Custom deleter for non-memory resources (e.g. C-style FILE* cleanup via fclose).
 * 3. Stateless functor vs function pointer impact on sizeof(std::unique_ptr).
 * 4. Contrast with std::shared_ptr where deleters are type-erased.
 */

#include <iostream>
#include <memory>
#include <cstdio>

// Custom stateless deleter struct for FILE*
struct FileCloser
{
    void operator()(std::FILE* fp) const
    {
        if (fp)
        {
            std::cout << "[FileCloser] Closing file resource safely.\n";
            std::fclose(fp);
        }
    }
};

int main()
{
    std::cout << "--- 1. Default Deleter Size ---\n";
    std::unique_ptr<int> defaultPtr = std::make_unique<int>(10);
    std::cout << "sizeof(unique_ptr<int>): " << sizeof(defaultPtr) << " bytes (same as raw pointer)\n\n";

    std::cout << "--- 2. Custom Deleter with Stateless Functor ---\n";
    {
        // Opening /dev/null as a dummy file stream for safe demonstration
        std::unique_ptr<std::FILE, FileCloser> filePtr(std::fopen("/dev/null", "r"));
        std::cout << "sizeof(unique_ptr with stateless functor): " << sizeof(filePtr) 
                  << " bytes (Empty Base Optimization preserves pointer size!)\n";
        // FileCloser is automatically invoked when filePtr goes out of scope
    }

    std::cout << "\n--- 3. Custom Deleter with Function Pointer ---\n";
    {
        // Function pointer deleter stores the function address inside unique_ptr
        std::unique_ptr<std::FILE, int(*)(std::FILE*)> filePtrWithFn(std::fopen("/dev/null", "r"), &std::fclose);
        std::cout << "sizeof(unique_ptr with function pointer): " << sizeof(filePtrWithFn) 
                  << " bytes (pointer size doubles: raw ptr + function ptr!)\n";
    }

    std::cout << "\n--- 4. Contrast: Type-erased Deleter in std::shared_ptr ---\n";
    {
        // For shared_ptr, the deleter is NOT part of the type signature!
        std::shared_ptr<std::FILE> sharedFile(std::fopen("/dev/null", "r"), [](std::FILE* fp) {
            if (fp)
            {
                std::cout << "[Lambda Deleter] Closing shared file.\n";
                std::fclose(fp);
            }
        });
        std::cout << "sizeof(shared_ptr<FILE>): " << sizeof(sharedFile) 
                  << " bytes (always 2 pointers: resource + control block)\n";
    }

    return 0;
}
