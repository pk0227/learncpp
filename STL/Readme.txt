============================================================
🚀 STL – INTERVIEW CHECKLIST (7+ YEARS C++ EXPERIENCE)
============================================================

⚠️ NOTE:
This is NOT full STL documentation.
This is what interviewers EXPECT you to know at a senior level.

============================================================
📦 1) IMPORTANT STL CONTAINERS
============================================================

------------------------------------------------------------
🧱 A) SEQUENCE CONTAINERS
------------------------------------------------------------

1️⃣ std::array
   🔹 Fixed-size, contiguous memory
   🔹 Random-access iterators
   🔹 Stack allocated
   🔹 No resize
   🔹 Zero overhead abstraction

2️⃣ std::vector
   🔹 Dynamic contiguous array
   🔹 Random-access iterators
   🔹 Reallocation possible
   🔹 Iterator invalidation on resize
   🔹 Cache-friendly (most used container)

3️⃣ std::deque
   🔹 Segmented contiguous blocks
   🔹 Random-access iterators
   🔹 Fast insertion at front & back
   🔹 Slightly slower traversal than vector

4️⃣ std::list
   🔹 Doubly linked list
   🔹 Bidirectional iterators
   🔹 Stable iterators
   🔹 High memory overhead
   🔹 Has list::sort() (merge sort)

5️⃣ std::forward_list
   🔹 Singly linked list
   🔹 Forward iterators only
   🔹 No size()
   🔹 Minimal memory footprint

------------------------------------------------------------
🌳 B) ASSOCIATIVE CONTAINERS (ORDERED)
------------------------------------------------------------

6️⃣ std::set
   🔹 Red-Black Tree
   🔹 Bidirectional iterators
   🔹 Ordered, unique keys
   🔹 O(log n)

7️⃣ std::map
   🔹 Red-Black Tree
   🔹 Bidirectional iterators
   🔹 Ordered key-value pairs
   🔹 O(log n) lookup

8️⃣ std::multiset
   🔹 Same as set
   🔹 Allows duplicate keys

9️⃣ std::multimap
   🔹 Same as map
   🔹 Allows duplicate keys

------------------------------------------------------------
⚡ C) UNORDERED CONTAINERS (HASH-BASED)
------------------------------------------------------------

🔟 std::unordered_set
1️⃣1️⃣ std::unordered_map
1️⃣2️⃣ std::unordered_multiset
1️⃣3️⃣ std::unordered_multimap

   🔹 Hash table + buckets
   🔹 Forward iterators
   🔹 Average O(1) operations
   🔹 Rehashing invalidates iterators
   🔹 No ordering guarantees

------------------------------------------------------------
🧩 D) CONTAINER ADAPTORS (CONCEPTUAL)
------------------------------------------------------------

1️⃣4️⃣ std::stack
   🔹 LIFO
   🔹 Built on deque/vector
   🔹 No iterators

1️⃣5️⃣ std::queue
   🔹 FIFO
   🔹 Built on deque

1️⃣6️⃣ std::priority_queue
   🔹 Heap-based
   🔹 Built on vector
   🔹 Max-heap by default

============================================================
🔁 2) ITERATORS (VERY IMPORTANT)
============================================================

📌 Iterator Categories:
🔹 Forward        → forward_list, unordered_*
🔹 Bidirectional  → list, map, set
🔹 Random-access  → vector, array, deque
🔹 Contiguous     → vector, array, string (C++20)

🧠 Must Know:
✔ iterator vs const_iterator
✔ reverse_iterator
✔ Iterator invalidation rules
✔ Why std::sort needs random-access iterators
✔ Why list::sort() exists

============================================================
🧮 3) CORE STL ALGORITHMS (INTERVIEW-RELEVANT)
============================================================

🔍 A) NON-MODIFYING
✔ std::find
✔ std::count / count_if
✔ std::all_of / any_of / none_of

🔄 B) MODIFYING
✔ std::copy
✔ std::move
✔ std::transform
✔ std::remove / remove_if
⚠️ ERASE–REMOVE IDIOM (VERY IMPORTANT)

📐 C) SORTING & SEARCHING
✔ std::sort
✔ std::stable_sort
✔ std::partial_sort
✔ std::binary_search
✔ std::lower_bound
✔ std::upper_bound
✔ std::equal_range

🧮 D) NUMERIC
✔ std::accumulate
✔ std::iota
✔ std::inner_product

🧱 E) HEAP
✔ std::make_heap
✔ std::push_heap
✔ std::pop_heap
✔ std::sort_heap

============================================================
🛠️ 4) FUNCTION OBJECTS & UTILITIES
============================================================

🎯 A) FUNCTORS / PREDICATES
✔ std::less, std::greater
✔ Custom comparators
✔ Lambdas (capture, mutable, noexcept)

🧰 B) UTILITY TYPES
✔ std::pair
✔ std::tuple
✔ std::optional
✔ std::variant
✔ std::any
✔ std::function (overhead awareness)

💾 C) MEMORY
✔ std::allocator (conceptual)
✔ Why STL uses allocators
✔ RAII principles

============================================================
🎯 5) CONTAINER SELECTION (CRITICAL FOR SENIORS)
============================================================

You MUST explain WHY:
✔ vector vs list
✔ map vs unordered_map
✔ deque vs vector
✔ set vs priority_queue
✔ array vs vector

🚨 Interviewers care more about
DECISION MAKING than implementation.

============================================================
❌ 6) WHAT IS NOT EXPECTED
============================================================

NOT EXPECTED:
❌ Implementing vector/map
❌ Writing tree rotations
❌ Coding hash tables

EXPECTED:
✔ Internal data structure knowledge
✔ Complexity guarantees
✔ Iterator behavior
✔ Correct STL usage in complex problems

============================================================
🏁 SENIOR-LEVEL ONE-LINER (INTERVIEW READY)
============================================================

STL is a generic C++ framework consisting of containers for storage,
iterators for traversal, algorithms for processing, and utilities for
customization. Senior-level interviews focus on internal behavior,
complexity, iterator guarantees, and correct container selection rather
than implementation.

============================================================
