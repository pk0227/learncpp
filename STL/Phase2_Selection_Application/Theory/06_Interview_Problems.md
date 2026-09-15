# 🏆 STL Interview Problems - Real Questions with Solutions

> **Practice Problems from Actual Senior C++ Interviews**

---

## 📑 Table of Contents

1. [Problem 1: LRU Cache (Design Problem) ⭐⭐⭐](#problem-1-lru-cache)
2. [Problem 2: Sliding Window Maximum (Deque Technique) ⭐⭐](#problem-2-sliding-window-maximum)
3. [Problem 3: Merge Intervals (Sorting + Merging) ⭐⭐](#problem-3-merge-intervals)
4. [Problem 4: Top K Frequent Elements (Heap / Bucket Sort) ⭐⭐](#problem-4-top-k-frequent-elements)
5. [Problem 5: Group Anagrams (Hash Map) ⭐](#problem-5-group-anagrams)
6. [Problem 6: LFU Cache (Advanced Design) ⭐⭐⭐](#problem-6-lfu-cache)
7. [Problem 7: Iterator Safety Bug (Debugging) ⭐⭐](#problem-7-find-the-bug)
8. [Problem 8: Meeting Rooms (Custom Comparator) ⭐](#problem-8-meeting-rooms)
9. [Problem 9: Range Sum Query - Mutable (Segment Tree) ⭐⭐](#problem-9-range-sum-query-mutable)
10. [Problem 10: Median from Data Stream (Two Heaps) ⭐⭐⭐](#problem-10-median-from-data-stream)
11. [🎓 Key Takeaways](#-key-takeaways)
12. [📁 Code Examples](#-code-examples)
13. [📚 Practice More](#-practice-more)

---

## Problem 1: LRU Cache

### Problem Statement
Design a data structure that follows the constraints of a **Least Recently Used (LRU) cache**.

Implement the `LRUCache` class:
- `LRUCache(int capacity)` - Initialize with positive capacity
- `int get(int key)` - Return value if exists, else -1
- `void put(int key, int value)` - Update or insert. If capacity exceeded, evict LRU item.

**Both operations must be O(1) average time complexity.**

### Example
```cpp
LRUCache cache(2);  // capacity = 2

cache.put(1, 1);  // cache: {1=1}
cache.put(2, 2);  // cache: {1=1, 2=2}
cache.get(1);     // returns 1, cache: {2=2, 1=1}
cache.put(3, 3);  // evicts key 2, cache: {1=1, 3=3}
cache.get(2);     // returns -1 (not found)
cache.put(4, 4);  // evicts key 1, cache: {3=3, 4=4}
```

### Container Selection Analysis

| Container | Get | Put | Evict | Why Not? |
|---|---|---|---|---|
| `vector` | $O(N)$ | $O(1)$ | $O(1)$ | ❌ Linear search |
| `map` | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ | ❌ Not $O(1)$ |
| `unordered_map` | $O(1)$ | $O(1)$ | $O(N)$ | ❌ Can't track LRU order |
| **`unordered_map` + `list`** | **$O(1)$** | **$O(1)$** | **$O(1)$** | ✅ Perfect! |

### Solution

```cpp
#include <list>
#include <unordered_map>
#include <utility>

class LRUCache {
private:
    int capacity;
    // List stores {key, value} pairs in LRU order (front = most recent)
    std::list<std::pair<int, int>> items;
    // Map: key -> iterator to position in list
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache;
    
public:
    LRUCache(int capacity) : capacity(capacity) {}
    
    int get(int key) {
        // Not found
        if (cache.find(key) == cache.end()) {
            return -1;
        }
        
        // Move accessed item to front (most recently used)
        items.splice(items.begin(), items, cache[key]);
        
        return cache[key]->second;
    }
    
    void put(int key, int value) {
        // Key exists - update and move to front
        if (cache.find(key) != cache.end()) {
            cache[key]->second = value;
            items.splice(items.begin(), items, cache[key]);
            return;
        }
        
        // Capacity check - evict LRU (back of list)
        if (cache.size() == capacity) {
            auto lru = items.back();
            cache.erase(lru.first);
            items.pop_back();
        }
        
        // Insert new item at front
        items.push_front({key, value});
        cache[key] = items.begin();
    }
};
```

### Why This Combination?

1. **`unordered_map`** - $O(1)$ lookup by key
2. **`list`** - $O(1)$ move to front, $O(1)$ remove from back, **iterator stability**
3. **Iterator stability** - Critical! Map stores iterators to list nodes; `list::splice` moves nodes without invalidating them.

### Interview Follow-ups

**Q: Why not `vector` instead of `list`?**  
A: Moving to front would be $O(N)$ with `vector` (need to shift elements). `list::splice` is $O(1)$.

**Q: Why not `deque` instead of `list`?**  
A: `deque` invalidates all iterators on middle operations. We need iterator stability to store iterators in the map.

**Q: What if we need thread-safety?**  
A: Add mutex locks (or read/write lock for `get`) around critical operations.

---

## Problem 2: Sliding Window Maximum

### Problem Statement
Given an array and a sliding window of size `k`, find the maximum in each window position.

### Example
```cpp
Input: nums = [1,3,-1,-3,5,3,6,7], k = 3
Output: [3,3,5,5,6,7]

Window positions:
[1  3  -1] -3  5  3  6  7  -> 3
 1 [3  -1  -3] 5  3  6  7  -> 3
 1  3 [-1  -3  5] 3  6  7  -> 5
 1  3  -1 [-3  5  3] 6  7  -> 5
 1  3  -1  -3 [5  3  6] 7  -> 6
 1  3  -1  -3  5 [3  6  7] -> 7
```

### Container Selection

| Approach | Container | Time | Why? |
|---|---|---|---|
| Naive | None | $O(NK)$ | Check max in each window |
| Heap | `priority_queue` | $O(N \log K)$ | ❌ Can't remove old elements efficiently |
| **Deque** | **`deque`** | **$O(N)$** | ✅ Maintain decreasing monotonic order |

### Solution (Deque Approach)

```cpp
#include <vector>
#include <deque>

std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
    std::vector<int> result;
    std::deque<int> dq;  // Stores indices, not values!
    
    for (int i = 0; i < nums.size(); ++i) {
        // Remove indices outside current window
        while (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        
        // Remove smaller elements (they'll never be max)
        while (!dq.empty() && nums[dq.back()] < nums[i]) {
            dq.pop_back();
        }
        
        dq.push_back(i);
        
        // Window is complete
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    
    return result;
}
```

### Why `deque`?

1. **Need front and back access** - Remove expired from front, add new at back
2. **Maintain decreasing order** - Remove smaller elements from back
3. **$O(1)$ amortized operations** at both ends

---

## Problem 3: Merge Intervals

### Problem Statement
Given a collection of intervals, merge all overlapping intervals.

### Example
```cpp
Input: [[1,3],[2,6],[8,10],[15,18]]
Output: [[1,6],[8,10],[15,18]]

Explanation: [1,3] and [2,6] overlap -> [1,6]
```

### Solution

```cpp
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> merge(std::vector<std::vector<int>>& intervals) {
    if (intervals.empty()) return {};
    
    // Sort by start time
    std::sort(intervals.begin(), intervals.end());
    
    std::vector<std::vector<int>> result;
    result.push_back(intervals[0]);
    
    for (size_t i = 1; i < intervals.size(); ++i) {
        // Overlaps with last interval
        if (intervals[i][0] <= result.back()[1]) {
            result.back()[1] = std::max(result.back()[1], intervals[i][1]);
        } else {
            // No overlap - add new interval
            result.push_back(intervals[i]);
        }
    }
    
    return result;
}
```

### Streaming Alternative with `std::map`

```cpp
#include <map>
#include <algorithm>

class IntervalMerger {
    std::map<int, int> intervals;  // start -> end
    
public:
    void addInterval(int start, int end) {
        auto it = intervals.lower_bound(start);
        
        // Merge with previous if overlapping
        if (it != intervals.begin()) {
            auto prev = std::prev(it);
            if (prev->second >= start) {
                start = prev->first;
                end = std::max(end, prev->second);
                intervals.erase(prev);
            }
        }
        
        // Merge with next intervals
        while (it != intervals.end() && it->first <= end) {
            end = std::max(end, it->second);
            it = intervals.erase(it);
        }
        
        intervals[start] = end;
    }
};
```

---

## Problem 4: Top K Frequent Elements

### Problem Statement
Given an integer array, return the `k` most frequent elements.

### Solution 1: Min-Heap (`std::priority_queue`)

```cpp
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>

std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
    std::unordered_map<int, int> freq;
    for (int num : nums) freq[num]++;
    
    // Min-heap of size k
    auto cmp = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<int, int>, 
                        std::vector<std::pair<int, int>>, 
                        decltype(cmp)> pq(cmp);
    
    for (const auto& [num, count] : freq) {
        pq.push({num, count});
        if (pq.size() > k) {
            pq.pop();
        }
    }
    
    std::vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top().first);
        pq.pop();
    }
    return result;
}
```
**Time:** $O(N \log K)$ | **Space:** $O(N)$

### Solution 2: Bucket Sort ($O(N)$ Time)

```cpp
#include <vector>
#include <unordered_map>

std::vector<int> topKFrequentBucket(std::vector<int>& nums, int k) {
    std::unordered_map<int, int> freq;
    for (int num : nums) freq[num]++;
    
    // Bucket sort: bucket[count] = elements with frequency count
    std::vector<std::vector<int>> buckets(nums.size() + 1);
    for (const auto& [num, count] : freq) {
        buckets[count].push_back(num);
    }
    
    std::vector<int> result;
    for (int i = buckets.size() - 1; i >= 0 && result.size() < k; --i) {
        for (int num : buckets[i]) {
            result.push_back(num);
            if (result.size() == k) break;
        }
    }
    return result;
}
```
**Time:** $O(N)$ | **Space:** $O(N)$

---

## Problem 5: Group Anagrams

### Solution

```cpp
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
    std::unordered_map<std::string, std::vector<std::string>> groups;
    
    for (const std::string& s : strs) {
        std::string key = s;
        std::sort(key.begin(), key.end());  // Canonical anagram signature
        groups[key].push_back(s);
    }
    
    std::vector<std::vector<std::string>> result;
    for (auto& [key, group] : groups) {
        result.push_back(std::move(group));
    }
    return result;
}
```

---

## Problem 6: LFU Cache

### Problem Statement
Design a **Least Frequently Used (LFU)** cache with $O(1)$ get and put operations.

### Solution

```cpp
#include <unordered_map>
#include <list>

class LFUCache {
private:
    int capacity, minFreq;
    struct Node { int key, value, freq; };
    
    std::unordered_map<int, std::list<Node>::iterator> keyNode;
    std::unordered_map<int, std::list<Node>> freqList;
    
public:
    LFUCache(int capacity) : capacity(capacity), minFreq(0) {}
    
    int get(int key) {
        if (keyNode.find(key) == keyNode.end()) return -1;
        
        auto node = *keyNode[key];
        freqList[node.freq].erase(keyNode[key]);
        
        if (freqList[node.freq].empty() && node.freq == minFreq) {
            minFreq++;
        }
        
        node.freq++;
        freqList[node.freq].push_front(node);
        keyNode[key] = freqList[node.freq].begin();
        
        return node.value;
    }
    
    void put(int key, int value) {
        if (capacity == 0) return;
        
        if (keyNode.find(key) != keyNode.end()) {
            keyNode[key]->value = value;
            get(key); // update frequency
            return;
        }
        
        if (keyNode.size() == capacity) {
            auto evict = freqList[minFreq].back();
            keyNode.erase(evict.key);
            freqList[minFreq].pop_back();
        }
        
        minFreq = 1;
        freqList[1].push_front({key, value, 1});
        keyNode[key] = freqList[1].begin();
    }
};
```

---

## Problem 7: Find the Bug

### Problem Statement
Find and fix the bug in this code:
```cpp
void removeEvenNumbers(std::vector<int>& v) {
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (*it % 2 == 0) {
            v.erase(it);  // ⚠️ Iterator invalidated! ++it is undefined behavior!
        }
    }
}
```

### Correct Solutions

```cpp
#include <vector>
#include <algorithm>

// Fix 1: Use erase return value (Single pass)
void removeEven_Fix1(std::vector<int>& v) {
    for (auto it = v.begin(); it != v.end(); ) {
        if (*it % 2 == 0) {
            it = v.erase(it);
        } else {
            ++it;
        }
    }
}

// Fix 2: Pre-C++20 erase-remove idiom
void removeEven_Fix2(std::vector<int>& v) {
    v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }), v.end());
}

// Fix 3: Modern C++20 uniform erasure (Best!)
void removeEven_Fix3(std::vector<int>& v) {
    std::erase_if(v, [](int x) { return x % 2 == 0; });
}
```

---

## Problem 8: Meeting Rooms

### Custom Comparator Example

```cpp
#include <vector>
#include <algorithm>
#include <string>

struct Meeting {
    int start, end;
    std::string title;
};

// Sort by start time, then by duration
bool canAttendMeetings(std::vector<Meeting>& meetings) {
    std::sort(meetings.begin(), meetings.end(), 
        [](const Meeting& a, const Meeting& b) {
            if (a.start != b.start) return a.start < b.start;
            return (a.end - a.start) < (b.end - b.start);
        });
        
    for (size_t i = 1; i < meetings.size(); ++i) {
        if (meetings[i].start < meetings[i - 1].end) {
            return false;
        }
    }
    return true;
}
```

---

## Problem 9: Range Sum Query (Mutable)

### Solution: Segment Tree using `std::vector`

```cpp
#include <vector>

class NumArray {
private:
    std::vector<int> tree;
    int n;
    
    void buildTree(const std::vector<int>& nums) {
        for (int i = 0; i < n; ++i) tree[n + i] = nums[i];
        for (int i = n - 1; i > 0; --i) tree[i] = tree[2 * i] + tree[2 * i + 1];
    }
    
public:
    NumArray(const std::vector<int>& nums) : n(nums.size()) {
        tree.resize(2 * n);
        buildTree(nums);
    }
    
    void update(int index, int val) {
        index += n;
        tree[index] = val;
        while (index > 1) {
            index /= 2;
            tree[index] = tree[2 * index] + tree[2 * index + 1];
        }
    }
    
    int sumRange(int left, int right) {
        left += n;
        right += n + 1;
        int sum = 0;
        while (left < right) {
            if (left % 2 == 1) sum += tree[left++];
            if (right % 2 == 1) sum += tree[--right];
            left /= 2;
            right /= 2;
        }
        return sum;
    }
};
```
**Time:** $O(\log N)$ for both `update` and `sumRange` | **Space:** $O(N)$

---

## Problem 10: Median from Data Stream

### Solution: Two Heaps

```cpp
#include <queue>
#include <vector>

class MedianFinder {
private:
    std::priority_queue<int> maxHeap; // Lower half
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // Upper half
    
public:
    void addNum(int num) {
        maxHeap.push(num);
        minHeap.push(maxHeap.top());
        maxHeap.pop();
        
        if (maxHeap.size() < minHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    double findMedian() const {
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        }
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};
```
**Time:** $O(\log N)$ add, $O(1)$ median | **Space:** $O(N)$

---

## 🎓 Key Takeaways

1. **LRU Cache:** `std::unordered_map` + `std::list` provides $O(1)$ operations via `list::splice`.
2. **Sliding Window:** `std::deque` provides $O(N)$ monotonic window management.
3. **Intervals:** Sort first, then merge sequentially.
4. **Top K:** Min-heap of size $K$ for $O(N \log K)$, or bucket sort for $O(N)$.
5. **Grouping:** `std::unordered_map` with sorted signature keys.
6. **Iterator Safety:** Use erase return value or C++20 `std::erase_if`.
7. **Custom Comparators:** Lambdas with Strict Weak Ordering (`<`).
8. **Median:** Two heaps (max + min) for $O(\log N)$ updates and $O(1)$ retrieval.

---

## 📁 Code Examples

- [`problem_01_lru_cache.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_01_lru_cache.cpp): LRU Cache implementation with `unordered_map` + `list`.
- [`problem_02_two_sum.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_02_two_sum.cpp): Two-sum hash lookup pattern.
- [`problem_03_valid_parentheses.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_03_valid_parentheses.cpp): LIFO bracket matching using `std::stack`.
- [`problem_04_top_k_frequent.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_04_top_k_frequent.cpp): Top-K frequent elements using min-heap and bucket sort.
- [`problem_05_group_anagrams.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_05_group_anagrams.cpp): String anagram bucketing with hash map.
- [`problem_06_file_system.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_06_file_system.cpp): In-memory hierarchical file system using tries/maps.
- [`problem_07_design_twitter.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_07_design_twitter.cpp): Twitter feed generation merging priority queues and maps.
- [`problem_08_merge_intervals.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_08_merge_intervals.cpp): Interval merging with batch vector sort and streaming map.

---

## 📚 Practice More

See [Interview Problems Catalog](../Code/interview_problems/README.md) for full runnable examples and build instructions!
