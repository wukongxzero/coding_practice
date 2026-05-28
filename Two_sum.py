# this would be your O(n_square) solution / brute force! 

from typing import List
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        for i in range(len(nums)):
            for j in range(len(nums)):
                if(nums[i]+nums[j] == target and i != j):
                    return [i,j]
    
sol = Solution()
print(sol.twoSum([2,7,11,15],9)) #expected [0,1]