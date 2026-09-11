/**
 * @file 2_enable_shared_from_this.cpp
 * @brief Demonstrates std::enable_shared_from_this and shared_from_this().
 *
 * Concepts covered:
 * 1. The problem with std::shared_ptr<T>(this): multiple control blocks causing double-free.
 * 2. The solution: inherit from std::enable_shared_from_this<T> and call shared_from_this().
 * 3. Precondition: the object must already be owned by an existing std::shared_ptr,
 *    otherwise shared_from_this() throws std::bad_weak_ptr.
 */

#include <iostream>
#include <memory>

class Worker : public std::enable_shared_from_this<Worker>
{
public:
    Worker() { std::cout << "Worker constructed\n"; }
    ~Worker() { std::cout << "Worker destroyed\n"; }

    // Correct way to produce a shared_ptr to *this
    std::shared_ptr<Worker> getSharedPtr()
    {
        return shared_from_this();
    }
};

int main()
{
    std::cout << "--- 1. Proper Usage with enable_shared_from_this ---\n";
    {
        std::shared_ptr<Worker> w1 = std::make_shared<Worker>();
        std::cout << "Initial count: " << w1.use_count() << '\n';

        // Obtain a second shared_ptr from inside the member function safely
        std::shared_ptr<Worker> w2 = w1->getSharedPtr();
        std::cout << "Count after getSharedPtr(): " << w1.use_count() << '\n';
    } // w1 and w2 both go out of scope, Worker destroyed exactly once!

    std::cout << "\n--- 2. Calling shared_from_this() without existing shared_ptr ownership ---\n";
    try
    {
        // Calling shared_from_this on a stack/raw object that is not managed by shared_ptr
        Worker stackWorker;
        auto badPtr = stackWorker.getSharedPtr(); // Throws std::bad_weak_ptr
    }
    catch (const std::bad_weak_ptr& e)
    {
        std::cout << "Caught expected std::bad_weak_ptr: " << e.what() << '\n';
    }

    return 0;
}
