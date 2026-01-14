# 🏆 STL Interview Problems - Real Questions with Solutions

> **Practice Problems from Actual Senior C++ Interviews**

---

## 📋 Problem Categories

1. [LRU Cache](#problem-1-lru-cache) - Design problem ⭐⭐⭐
2. [Sliding Window Maximum](#problem-2-sliding-window-maximum) - Deque technique ⭐⭐
3. [Merge Intervals](#problem-3-merge-intervals) - Sorting + merging ⭐⭐
4. [Top K Frequent Elements](#problem-4-top-k-frequent-elements) - Heap/map ⭐⭐
5. [Group Anagrams](#problem-5-group-anagrams) - Hash map ⭐
6. [LFU Cache](#problem-6-lfu-cache) - Advanced design ⭐⭐⭐
7. [Iterator Safety Bug](#problem-7-find-the-bug) - Debugging ⭐⭐
8. [Custom Comparator](#problem-8-meeting-rooms) - Sorting ⭐
9. [Range Sum Query](#problem-9-range-sum-query-mutable) - Data structure design ⭐⭐
10. [Median from Data Stream](#problem-10-median-from-data-stream) - Two heaps ⭐⭐⭐

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
|-----------|-----|-----|-------|----------|
| `vector` | O(n) | O(1) | O(1) | ❌ Linear search |
| `map` | O(log n) | O(log n) | O(log n) | ❌ Not O(1) |
| `unordered_map` | O(1) | O(1) | O(n) | ❌ Can't track LRU order |
| **`unordered_map` + `list`** | **O(1)** | **O(1)** | **O(1)** | ✅ Perfect! |

### Solution

```cpp
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

1. **`unordered_map`** - O(1) lookup by key
2. **`list`** - O(1) move to front, O(1) remove from back, **iterator stability**
3. **Iterator stability** - Critical! Map stores iterators to list nodes

### Interview Follow-ups

**Q: Why not `vector` instead of `list`?**  
A: Moving to front would be O(n) with `vector` (need to shift elements). `list::splice` is O(1).

**Q: Why not `deque` instead of `list`?**  
A: `deque` invalidates all iterators on middle operations. We need iterator stability to store iterators in the map.

**Q: What if we need thread-safety?**  
A: Add mutex locks around all operations.

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
|----------|-----------|------|------|
| Naive | None | O(nk) | Check max in each window |
| Heap | `priority_queue` | O(n log k) | ❌ Can't remove old elements efficiently |
| **Deque** | **`deque`** | **O(n)** | ✅ Maintain decreasing order |

### Solution (Deque Approach)

```cpp
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

1. **Need front and back access** - Remove old from front, add new at back
2. **Maintain decreasing order** - Remove smaller elements from back
3. **O(1) operations** at both ends

### Interview Follow-ups

**Q: Why not `priority_queue`?**  
A: Can't efficiently remove elements that slide out of window. Would need to track which elements are stale.

**Q: Why store indices instead of values?**  
A: Need to check if element is still in window (compare index with `i - k`).

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
std::vector<std::vector<int>> merge(std::vector<std::vector<int>>& intervals) {
    if (intervals.empty()) return {};
    
    // Sort by start time
    std::sort(intervals.begin(), intervals.end());
    
    std::vector<std::vector<int>> result;
    result.push_back(intervals[0]);
    
    for (int i = 1; i < intervals.size(); ++i) {
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

### Container Choice: `vector`

**Why?**
- Need to sort - requires random-access iterators
- Sequential processing after sorting
- Result is also sequential

### Interview Follow-ups

**Q: What if intervals arrive one at a time (streaming)?**  
A: Use `set` or `map` to maintain sorted order, merge on insertion.

```cpp
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

### Example
```cpp
Input: nums = [1,1,1,2,2,3], k = 2
Output: [1,2]
```

### Solution 1: Min-Heap (Priority Queue)

```cpp
std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
    // Count frequencies
    std::unordered_map<int, int> freq;
    for (int num : nums) {
        freq[num]++;
    }
    
    // Min-heap of size k (keep k largest frequencies)
    auto cmp = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;  // Min-heap
    };
    std::priority_queue<std::pair<int, int>, 
                        std::vector<std::pair<int, int>>, 
                        decltype(cmp)> pq(cmp);
    
    for (const auto& [num, count] : freq) {
        pq.push({num, count});
        if (pq.size() > k) {
            pq.pop();  // Remove smallest
        }
    }
    
    // Extract results
    std::vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top().first);
        pq.pop();
    }
    
    return result;
}
```

**Time:** O(n log k)  
**Space:** O(n)

### Solution 2: Bucket Sort (Faster!)

```cpp
std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
    // Count frequencies
    std::unordered_map<int, int> freq;
    for (int num : nums) {
        freq[num]++;
    }
    
    // Bucket sort: bucket[i] = elements with frequency i
    std::vector<std::vector<int>> buckets(nums.size() + 1);
    for (const auto& [num, count] : freq) {
        buckets[count].push_back(num);
    }
    
    // Collect top k from highest frequency buckets
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

**Time:** O(n)  
**Space:** O(n)

### Container Comparison

| Approach | Containers | Time | When to Use |
|----------|-----------|------|-------------|
| Min-heap | `unordered_map` + `priority_queue` | O(n log k) | General purpose |
| Bucket sort | `unordered_map` + `vector` | O(n) | Frequency range is small |
| Full sort | `unordered_map` + `vector` | O(n log n) | Need all elements sorted |

---

## Problem 5: Group Anagrams

### Problem Statement
Group strings that are anagrams of each other.

### Example
```cpp
Input: ["eat","tea","tan","ate","nat","bat"]
Output: [["eat","tea","ate"],["tan","nat"],["bat"]]
```

### Solution

```cpp
std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
    std::unordered_map<std::string, std::vector<std::string>> groups;
    
    for (const std::string& s : strs) {
        std::string key = s;
        std::sort(key.begin(), key.end());  // Sorted string as key
        groups[key].push_back(s);
    }
    
    std::vector<std::vector<std::string>> result;
    for (auto& [key, group] : groups) {
        result.push_back(std::move(group));
    }
    
    return result;
}
```

### Why `unordered_map`?

- **Key:** Sorted string (anagram signature)
- **Value:** Vector of original strings
- **O(1) average lookup** for grouping

### Alternative: Character Count as Key

```cpp
std::string getKey(const std::string& s) {
    std::array<int, 26> count = {};
    for (char c : s) {
        count[c - 'a']++;
    }
    
    std::string key;
    for (int i = 0; i < 26; ++i) {
        if (count[i] > 0) {
            key += std::string(count[i], 'a' + i);
        }
    }
    return key;
}
```

---

## Problem 6: LFU Cache

### Problem Statement
Design a **Least Frequently Used (LFU)** cache with O(1) operations.

When there's a tie in frequency, evict the **least recently used** among them.

### Solution (Advanced!)

```cpp
class LFUCache {
private:
    int capacity, minFreq;
    
    struct Node {
        int key, value, freq;
    };
    
    std::unordered_map<int, std::list<Node>::iterator> keyNode;
    std::unordered_map<int, std::list<Node>> freqList;
    
public:
    LFUCache(int capacity) : capacity(capacity), minFreq(0) {}
    
    int get(int key) {
        if (keyNode.find(key) == keyNode.end()) {
            return -1;
        }
        
        // Update frequency
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
        
        // Key exists - update
        if (keyNode.find(key) != keyNode.end()) {
            auto node = *keyNode[key];
            node.value = value;
            get(key);  // Update frequency
            keyNode[key]->value = value;
            return;
        }
        
        // Evict if full
        if (keyNode.size() == capacity) {
            auto evict = freqList[minFreq].back();
            keyNode.erase(evict.key);
            freqList[minFreq].pop_back();
        }
        
        // Insert new
        minFreq = 1;
        freqList[1].push_front({key, value, 1});
        keyNode[key] = freqList[1].begin();
    }
};
```

### Container Design

- **`unordered_map<int, iterator>`** - Key to node lookup (O(1))
- **`unordered_map<int, list<Node>>`** - Frequency to nodes (O(1))
- **`list`** - LRU order within same frequency (O(1) operations)

---

## Problem 7: Find the Bug

### Problem Statement
Find and fix the bug in this code:

```cpp
void removeEvenNumbers(std::vector<int>& v) {
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (*it % 2 == 0) {
            v.erase(it);
        }
    }
}
```

### Bug Analysis

**Problem:** Iterator invalidation!
- `erase` invalidates `it`
- `++it` in loop uses invalidated iterator → **undefined behavior**

### Fixed Version

```cpp
void removeEvenNumbers(std::vector<int>& v) {
    for (auto it = v.begin(); it != v.end(); ) {
        if (*it % 2 == 0) {
            it = v.erase(it);  // erase returns next valid iterator
        } else {
            ++it;
        }
    }
}

// Or better: erase-remove idiom
void removeEvenNumbers(std::vector<int>& v) {
    v.erase(std::remove_if(v.begin(), v.end(),
        [](int x) { return x % 2 == 0; }), v.end());
}
```

---

## Problem 8: Meeting Rooms

### Problem Statement
Given meeting time intervals, determine if a person can attend all meetings.

### Example
```cpp
Input: [[0,30],[5,10],[15,20]]
Output: false (conflicts: [0,30] overlaps with others)

Input: [[7,10],[2,4]]
Output: true (no conflicts)
```

### Solution

```cpp
bool canAttendMeetings(std::vector<std::vector<int>>& intervals) {
    // Sort by start time
    std::sort(intervals.begin(), intervals.end());
    
    for (int i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] < intervals[i-1][1]) {
            return false;  // Overlap detected
        }
    }
    
    return true;
}
```

### Custom Comparator Example

```cpp
struct Meeting {
    int start, end;
    std::string title;
};

// Sort by start time, then by duration
std::vector<Meeting> meetings = /* ... */;

std::sort(meetings.begin(), meetings.end(), 
    [](const Meeting& a, const Meeting& b) {
        if (a.start != b.start) {
            return a.start < b.start;
        }
        return (a.end - a.start) < (b.end - b.start);
    });
```

---

## Problem 9: Range Sum Query (Mutable)

### Problem Statement
Given an array, support:
- `update(index, val)` - Update element
- `sumRange(left, right)` - Return sum of range

Both operations should be efficient.

### Solution: Segment Tree (Using `vector`)

```cpp
class NumArray {
private:
    std::vector<int> tree;
    int n;
    
    void buildTree(const std::vector<int>& nums) {
        for (int i = 0; i < n; ++i) {
            tree[n + i] = nums[i];
        }
        for (int i = n - 1; i > 0; --i) {
            tree[i] = tree[2*i] + tree[2*i + 1];
        }
    }
    
public:
    NumArray(std::vector<int>& nums) : n(nums.size()) {
        tree.resize(2 * n);
        buildTree(nums);
    }
    
    void update(int index, int val) {
        index += n;
        tree[index] = val;
        
        while (index > 1) {
            index /= 2;
            tree[index] = tree[2*index] + tree[2*index + 1];
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

**Time:** O(log n) for both operations  
**Space:** O(n)

---

## Problem 10: Median from Data Stream

### Problem Statement
Design a data structure that supports:
- `addNum(int num)` - Add number to stream
- `findMedian()` - Return median of all numbers

### Solution: Two Heaps

```cpp
class MedianFinder {
private:
    std::priority_queue<int> maxHeap;  // Left half (smaller elements)
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;  // Right half
    
public:
    void addNum(int num) {
        // Add to max heap first
        maxHeap.push(num);
        
        // Balance: move largest from left to right
        minHeap.push(maxHeap.top());
        maxHeap.pop();
        
        // Maintain size: left >= right
        if (maxHeap.size() < minHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    double findMedian() {
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        }
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};
```

**Time:** O(log n) add, O(1) median  
**Space:** O(n)

### Why Two Heaps?

- **Max heap (left):** Stores smaller half, top = largest of small
- **Min heap (right):** Stores larger half, top = smallest of large
- **Median:** Either max heap top (odd count) or average of both tops (even count)

---

## 🎓 Key Takeaways

1. **LRU Cache:** `unordered_map` + `list` for O(1) operations
2. **Sliding Window:** `deque` for efficient front/back access
3. **Intervals:** Sort first, then merge sequentially
4. **Top K:** Min-heap of size k, or bucket sort for O(n)
5. **Grouping:** `unordered_map` with computed keys
6. **Iterator Safety:** Always use erase return value
7. **Custom Comparators:** Lambdas for flexible sorting
8. **Median:** Two heaps (max + min) for O(log n) updates

---

## 📚 Practice More

See [examples/interview_problems/](file:///home/prashanth/learncpp_workspace/STL/examples/interview_problems/) for compilable code!

---

**Remember:** Justify your container choices in interviews. Explain trade-offs!
