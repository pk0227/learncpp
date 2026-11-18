// 14_3_Const_class_objects_n_const_member_functions/19_mutable_mutex_thread_safety.cpp
// Demonstrates why mutable members need synchronization: "const means thread-safe".
//
// The C++ standard library guarantees that calling const member functions on the same object
// from several threads at once is race-free (it assumes const functions only READ).
// Your own types are expected to keep the same promise.
//
// A mutable member breaks that assumption: two threads in the same const function can
// both WRITE it -> data race -> undefined behavior.
// Fix ("M&M rule": mutable and mutex go together):
//   - protect mutable state with a  mutable std::mutex  (lock() is non-const, so the mutex
//     itself must be mutable to be locked inside a const function), or
//   - use  mutable std::atomic<T>  for a single counter.
#include <atomic>
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

class Polygon
{
    double m_width{};
    double m_height{};
    mutable std::mutex m_mutex{};                    // protects m_cachedArea
    mutable std::optional<double> m_cachedArea{};
    mutable std::atomic<int> m_calls{ 0 };           // single counter: atomic is enough

public:
    Polygon(double w, double h) : m_width{ w }, m_height{ h } {}

    double area() const
    {
        ++m_calls;                                   // atomic increment, no lock needed
        std::lock_guard<std::mutex> lock{ m_mutex }; // OK: m_mutex is mutable
        if (!m_cachedArea)
            m_cachedArea = m_width * m_height;
        return *m_cachedArea;
    }

    int calls() const { return m_calls.load(); }
};

int main()
{
    const Polygon shared{ 3.0, 4.0 };   // one const object, shared by many threads

    std::vector<std::thread> workers{};
    for (int i{ 0 }; i < 8; ++i)
        workers.emplace_back([&shared] {
            for (int j{ 0 }; j < 1000; ++j)
                shared.area();
        });

    for (auto& t : workers)
        t.join();

    std::cout << "area: " << shared.area() << '\n';
    std::cout << "calls: " << shared.calls() << '\n';   // always 8001: no lost updates
    return 0;
}
