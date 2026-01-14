/**
 * Problem 07: Design Twitter - Multi-Container System Design
 *
 * Design a simplified version of Twitter where users can post tweets,
 * follow/unfollow another user, and see the 10 most recent tweets in their news
 * feed.
 *
 * Senior-level focus: Multiple containers working together, time-based
 * ordering, efficient merge of sorted lists
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 problem_07_design_twitter.cpp -o
 * design_twitter Run: ./design_twitter
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// ============================================================================
// SOLUTION
// ============================================================================

class Twitter {
private:
  struct Tweet {
    int tweetId;
    int timestamp;

    bool operator<(const Tweet &other) const {
      return timestamp < other.timestamp; // For max-heap
    }
  };

  int globalTime;

  // userId -> list of tweets (most recent first)
  std::unordered_map<int, std::vector<Tweet>> tweets;

  // userId -> set of followees
  std::unordered_map<int, std::unordered_set<int>> following;

public:
  Twitter() : globalTime(0) {}

  // Post a tweet
  void postTweet(int userId, int tweetId) {
    tweets[userId].push_back({tweetId, globalTime++});
  }

  // Get 10 most recent tweets from user and followees
  std::vector<int> getNewsFeed(int userId) {
    // Max-heap to merge sorted lists
    std::priority_queue<std::pair<int, std::pair<int, int>>> pq;
    // {timestamp, {userId, index}}

    // Add user's own tweets
    if (!tweets[userId].empty()) {
      int idx = tweets[userId].size() - 1;
      pq.push({tweets[userId][idx].timestamp, {userId, idx}});
    }

    // Add followees' tweets
    for (int followeeId : following[userId]) {
      if (!tweets[followeeId].empty()) {
        int idx = tweets[followeeId].size() - 1;
        pq.push({tweets[followeeId][idx].timestamp, {followeeId, idx}});
      }
    }

    // Extract top 10
    // Extract top 10
    std::vector<int> result;
    while (!pq.empty() && result.size() < 10u) {
      auto [timestamp, userIdx] = pq.top();
      pq.pop();

      auto [uid, idx] = userIdx;
      result.push_back(tweets[uid][idx].tweetId);

      // Add next tweet from same user
      if (idx > 0) {
        pq.push({tweets[uid][idx - 1].timestamp, {uid, idx - 1}});
      }
    }

    return result;
  }

  // Follow a user
  void follow(int followerId, int followeeId) {
    if (followerId != followeeId) { // Can't follow self
      following[followerId].insert(followeeId);
    }
  }

  // Unfollow a user
  void unfollow(int followerId, int followeeId) {
    following[followerId].erase(followeeId);
  }
};

// ============================================================================
// CONTAINER SELECTION ANALYSIS
// ============================================================================

/*
 * MULTI-CONTAINER DESIGN (Senior-level insight)
 *
 * Requirements:
 * - Store tweets per user (time-ordered)
 * - Store follow relationships
 * - Merge K sorted lists efficiently
 *
 * Container Choices:
 *
 * 1. Tweets: unordered_map<int, vector<Tweet>>
 *    - Why unordered_map? O(1) access by userId
 *    - Why vector? Tweets are time-ordered, append-only
 *    - Alternative: list? No - don't need insertion in middle
 *
 * 2. Following: unordered_map<int, unordered_set<int>>
 *    - Why unordered_map? O(1) access by userId
 *    - Why unordered_set? O(1) follow/unfollow, no duplicates
 *    - Alternative: vector? No - need O(1) erase for unfollow
 *
 * 3. News Feed: priority_queue (max-heap)
 *    - Merge K sorted lists (K = number of followees)
 *    - Each user's tweets are sorted by time
 *    - Heap efficiently merges: O(10 log K)
 *
 * SENIOR INSIGHT: This problem combines multiple containers,
 * each chosen for specific operations. Understanding the
 * trade-offs is crucial.
 *
 * Alternative Approach: Store all tweets in one sorted structure
 * - Would be O(n log n) for each post
 * - Current approach: O(1) post, O(K log K) for feed
 */

// ============================================================================
// TEST CASES
// ============================================================================

void testBasicOperations() {
  std::cout << "=== TEST: Basic Operations ===\n";
  Twitter twitter;

  twitter.postTweet(1, 5);
  auto feed = twitter.getNewsFeed(1);
  std::cout << "   User 1 posts tweet 5\n";
  std::cout << "   News feed: ";
  for (int id : feed)
    std::cout << id << " ";
  std::cout << "\n";
  assert(feed.size() == 1 && feed[0] == 5);

  twitter.follow(1, 2);
  twitter.postTweet(2, 6);
  feed = twitter.getNewsFeed(1);
  std::cout << "   User 1 follows user 2, user 2 posts 6\n";
  std::cout << "   News feed: ";
  for (int id : feed)
    std::cout << id << " ";
  std::cout << "\n";
  assert(feed.size() == 2);

  std::cout << "   ✓ Basic operations passed\n\n";
}

void testUnfollow() {
  std::cout << "=== TEST: Unfollow ===\n";
  Twitter twitter;

  twitter.postTweet(1, 5);
  twitter.follow(1, 2);
  twitter.postTweet(2, 6);
  twitter.unfollow(1, 2);

  auto feed = twitter.getNewsFeed(1);
  std::cout << "   After unfollow, news feed: ";
  for (int id : feed)
    std::cout << id << " ";
  std::cout << "\n";
  assert(feed.size() == 1 && feed[0] == 5);

  std::cout << "   ✓ Unfollow test passed\n\n";
}

void testTimeOrdering() {
  std::cout << "=== TEST: Time Ordering ===\n";
  Twitter twitter;

  twitter.postTweet(1, 1);
  twitter.postTweet(2, 2);
  twitter.postTweet(1, 3);
  twitter.follow(1, 2);

  auto feed = twitter.getNewsFeed(1);
  std::cout << "   News feed (most recent first): ";
  for (int id : feed)
    std::cout << id << " ";
  std::cout << "\n";
  assert(feed[0] == 3); // Most recent
  assert(feed[1] == 2);
  assert(feed[2] == 1); // Oldest

  std::cout << "   ✓ Time ordering test passed\n\n";
}

void testTop10Limit() {
  std::cout << "=== TEST: Top 10 Limit ===\n";
  Twitter twitter;

  // Post 15 tweets
  for (int i = 1; i <= 15; ++i) {
    twitter.postTweet(1, i);
  }

  auto feed = twitter.getNewsFeed(1);
  std::cout << "   Posted 15 tweets, feed size: " << feed.size() << "\n";
  std::cout << "   Feed: ";
  for (int id : feed)
    std::cout << id << " ";
  std::cout << "\n";
  assert(feed.size() == 10);
  assert(feed[0] == 15); // Most recent
  assert(feed[9] == 6);  // 10th most recent

  std::cout << "   ✓ Top 10 limit test passed\n\n";
}

void testMultipleFollowees() {
  std::cout << "=== TEST: Multiple Followees ===\n";
  Twitter twitter;

  twitter.postTweet(1, 1);
  twitter.postTweet(2, 2);
  twitter.postTweet(3, 3);
  twitter.postTweet(4, 4);

  twitter.follow(1, 2);
  twitter.follow(1, 3);
  twitter.follow(1, 4);

  auto feed = twitter.getNewsFeed(1);
  std::cout << "   Following 3 users, feed: ";
  for (int id : feed)
    std::cout << id << " ";
  std::cout << "\n";
  assert(feed.size() == 4);

  std::cout << "   ✓ Multiple followees test passed\n\n";
}

// ============================================================================
// COMPLEXITY ANALYSIS
// ============================================================================

void complexityAnalysis() {
  std::cout << "=== COMPLEXITY ANALYSIS ===\n\n";

  std::cout << "Let K = number of followees, N = total tweets\n\n";

  std::cout << "postTweet(userId, tweetId):\n";
  std::cout << "  Time: O(1) - append to vector\n";
  std::cout << "  Space: O(1)\n\n";

  std::cout << "getNewsFeed(userId):\n";
  std::cout << "  Time: O(K log K + 10 log K) = O(K log K)\n";
  std::cout << "    - Build initial heap: O(K log K)\n";
  std::cout << "    - Extract 10 elements: O(10 log K)\n";
  std::cout << "  Space: O(K) for heap\n\n";

  std::cout << "follow(followerId, followeeId):\n";
  std::cout << "  Time: O(1) average - hash set insert\n";
  std::cout << "  Space: O(1)\n\n";

  std::cout << "unfollow(followerId, followeeId):\n";
  std::cout << "  Time: O(1) average - hash set erase\n";
  std::cout << "  Space: O(1)\n\n";
}

// ============================================================================
// SENIOR-LEVEL INSIGHTS
// ============================================================================

void seniorInsights() {
  std::cout << "=== SENIOR-LEVEL INSIGHTS ===\n\n";

  std::cout << "1. Multi-Container Coordination:\n";
  std::cout << "   - 3 different containers working together\n";
  std::cout << "   - Each optimized for specific operations\n";
  std::cout << "   - Understanding when to use which container\n\n";

  std::cout << "2. Merge K Sorted Lists Pattern:\n";
  std::cout << "   - Classic algorithm problem\n";
  std::cout << "   - Priority queue for efficient merging\n";
  std::cout << "   - Each user's tweets are pre-sorted by time\n\n";

  std::cout << "3. Time Complexity Trade-offs:\n";
  std::cout << "   - Fast post (O(1)) vs slower feed (O(K log K))\n";
  std::cout << "   - Acceptable because posts >> feed reads\n";
  std::cout << "   - Could cache feeds for further optimization\n\n";

  std::cout << "4. Interview Follow-ups:\n";
  std::cout << "   - How to handle millions of users? (sharding)\n";
  std::cout << "   - How to cache news feeds? (Redis, memcached)\n";
  std::cout << "   - How to handle retweets? (add retweet timestamp)\n";
  std::cout << "   - How to implement likes/comments? (separate maps)\n";
  std::cout << "   - How to scale horizontally? (partition by userId)\n\n";

  std::cout << "5. Real-World Considerations:\n";
  std::cout << "   - Database persistence (not in-memory)\n";
  std::cout << "   - Eventual consistency\n";
  std::cout << "   - Rate limiting\n";
  std::cout << "   - Spam detection\n\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "Design Twitter\n";
  std::cout << "==============\n\n";

  testBasicOperations();
  testUnfollow();
  testTimeOrdering();
  testTop10Limit();
  testMultipleFollowees();
  complexityAnalysis();
  seniorInsights();

  std::cout << "All tests passed! ✓\n";
  std::cout << "\nThis problem tests:\n";
  std::cout << "- Multi-container system design\n";
  std::cout << "- Merge K sorted lists with heap\n";
  std::cout << "- Time-based ordering\n";
  std::cout << "- Hash table for relationships\n";
  std::cout << "- System design thinking\n";
  std::cout << "- Scalability considerations\n";

  return 0;
}
