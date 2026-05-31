from typing import List 
class Solution:
    def twosum(self, nums : List[int],target: int) -> List[int]:
        seen = {}
        for i in range(len(nums)):
            complement = target - nums[i]
            if complement in seen : 
                return [seen[complement],i]
            seen[nums[i]] = i
sol = Solution()
print(sol.twosum([2,7,11,15],9))
