#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

// Top K Frequent Elements — NeetCode #5
// Approach: hashmap for counts + min-heap of size k
// Time: O(n log k)  Space: O(n)

vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int n : nums)
        freq[n]++;

    // min heap: {count, value} — smallest count at top
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> minHeap;

    for (auto& [val, cnt] : freq) {
        minHeap.push({cnt, val});
        if (minHeap.size() > k)
            minHeap.pop();  // evict least frequent, keeping only top k
    }

    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top().second);
        minHeap.pop();
    }
    return result;
}

#include <iostream>
int main() {
    vector<int> nums1 = {1,1,1,2,2,3};
    auto r1 = topKFrequent(nums1, 2);
    for (int x : r1) cout << x << " ";
    cout << "\n";  // expected: 1 2 (any order)

    vector<int> nums2 = {1};
    auto r2 = topKFrequent(nums2, 1);
    for (int x : r2) cout << x << " ";
    cout << "\n";  // expected: 1
}
