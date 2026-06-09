#include<vector>
#include<iostream>

// Time: O(n) — single pass two pointers
// Space: O(1)
vector<int> two_sum(vector<int>& nums, int target){
    int left = 0;
    int right = nums.size() - 1;

    while (left < right){
        int sum = nums[left] + nums[right];
        if (sum < target){
            left++;
        }
        if (sum > target){
            right--;
        }
        if (sum == target){
            return {left+1, right+1};
        }
    }
    return {};
}
