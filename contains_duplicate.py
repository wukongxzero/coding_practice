from typing import List
class Solution:
    def hasDuplicate(self, nums: List[int]) -> bool:
        for i in range(len(nums)):
            for j in range(len(nums)):
                if (i!=j and nums[i] == nums[j]):
                    return True
                 
        return False 

sol = Solution()
print(sol.hasDuplicate([1,2,3,1]))
print(sol.hasDuplicate([1,2,3,4]))