// 14_3_Const_class_objects_n_const_member_functions/18_mutable_cache_lazy_evaluation.cpp
// Demonstrates the classic mutable use case: caching an expensive result inside a const getter.
//
// The cache is NOT part of what the object means. Whether it is filled or empty,
// the caller sees the same value. So the getter stays logically const.
// The cache must be invalidated whenever the real data changes.
//
// NOTE: this version is NOT thread-safe. Two threads calling area() on the same
// const object can race on the cache. See 19_mutable_mutex_thread_safety.cpp.
#include <iostream>
#include <optional>

class Polygon
{
    double m_width{};
    double m_height{};
    mutable std::optional<double> m_cachedArea{};   // hidden bookkeeping

public:
    Polygon(double w, double h) : m_width{ w }, m_height{ h } {}

    double area() const
    {
        if (!m_cachedArea)
        {
            std::cout << "  (computing area)\n";    // pretend this is expensive
            m_cachedArea = m_width * m_height;      // OK: mutable member written in const function
        }
        return *m_cachedArea;
    }

    void setWidth(double w)
    {
        m_width = w;
        m_cachedArea.reset();                       // invalidate: real data changed
    }
};

int main()
{
    Polygon p{ 3.0, 4.0 };
    std::cout << p.area() << '\n';   // computes
    std::cout << p.area() << '\n';   // cached
    p.setWidth(5.0);
    std::cout << p.area() << '\n';   // recomputes after invalidation

    const Polygon cp{ 2.0, 2.0 };
    std::cout << cp.area() << '\n';  // caching works on a const object too
    return 0;
}
