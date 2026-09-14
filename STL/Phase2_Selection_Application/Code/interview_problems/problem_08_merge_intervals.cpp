/**
 * Problem 08: Merge Intervals & Streaming Interval Merger
 *
 * Difficulty: Medium
 * Containers Used: std::vector (batch sorting), std::map (streaming insertions)
 *
 * Interview Focus:
 * - When intervals are known up-front: sort vector by start time in O(N log N)
 * - When intervals arrive in a stream: maintain non-overlapping intervals in std::map with O(log N) merge
 *
 * Compile:
 *   g++ -std=c++20 -O2 -Wall -Wextra problem_08_merge_intervals.cpp -o problem_08
 */

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

struct Interval {
    int start;
    int end;

    bool operator==(const Interval& other) const {
        return start == other.start && end == other.end;
    }
};

// ============================================================================
// BATCH APPROACH: std::vector + std::sort (O(N log N))
// ============================================================================

std::vector<Interval> mergeIntervals(std::vector<Interval> intervals) {
    if (intervals.empty()) return {};

    // 1. Sort intervals by start time
    std::sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
        return a.start < b.start;
    });

    std::vector<Interval> merged;
    merged.push_back(intervals[0]);

    for (size_t i = 1; i < intervals.size(); ++i) {
        // If current interval overlaps with the last merged interval
        if (intervals[i].start <= merged.back().end) {
            merged.back().end = std::max(merged.back().end, intervals[i].end);
        } else {
            merged.push_back(intervals[i]);
        }
    }

    return merged;
}

// ============================================================================
// STREAMING APPROACH: std::map (O(log N) insert & merge)
// ============================================================================

class StreamingIntervalMerger {
    // Map: start -> end
    std::map<int, int> intervals;

public:
    void addInterval(int start, int end) {
        auto it = intervals.lower_bound(start);

        // Check and merge with preceding overlapping interval
        if (it != intervals.begin()) {
            auto prev = std::prev(it);
            if (prev->second >= start) {
                start = prev->first;
                end = std::max(end, prev->second);
                intervals.erase(prev);
            }
        }

        // Merge with all succeeding overlapping intervals
        while (it != intervals.end() && it->first <= end) {
            end = std::max(end, it->second);
            it = intervals.erase(it);
        }

        intervals[start] = end;
    }

    std::vector<Interval> getIntervals() const {
        std::vector<Interval> result;
        for (const auto& [start, end] : intervals) {
            result.push_back({start, end});
        }
        return result;
    }
};

// ============================================================================
// MAIN & VERIFICATION
// ============================================================================

int main() {
    std::cout << "=== Problem 08: Merge Intervals Demo ===\n\n";

    // Test 1: Batch merging
    std::vector<Interval> input = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
    auto output = mergeIntervals(input);

    std::cout << "Batch merged intervals:\n";
    for (const auto& iv : output) {
        std::cout << "  [" << iv.start << ", " << iv.end << "]\n";
    }

    assert(output.size() == 3);
    assert((output[0] == Interval{1, 6}));
    assert((output[1] == Interval{8, 10}));
    assert((output[2] == Interval{15, 18}));

    // Test 2: Streaming merging
    std::cout << "\nStreaming merged intervals:\n";
    StreamingIntervalMerger stream;
    stream.addInterval(1, 4);
    stream.addInterval(8, 12);
    stream.addInterval(3, 9); // Merges [1,4] and [8,12] into [1,12]

    auto streamOutput = stream.getIntervals();
    for (const auto& iv : streamOutput) {
        std::cout << "  [" << iv.start << ", " << iv.end << "]\n";
    }

    assert(streamOutput.size() == 1);
    assert((streamOutput[0] == Interval{1, 12}));

    std::cout << "\nAll interval tests passed successfully!\n";
    return 0;
}
