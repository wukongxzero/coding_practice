#include <vector>
#include <iostream>
using namespace std;

vector<int> ProductExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> output(n, 1);

    int prefix = 1;
    for (int i = 0; i < n; i++) {
        output[i] = prefix;
        prefix *= nums[i];
    }

    int suffix = 1;
    for (int i = n - 1; i >= 0; i--) {
        output[i] *= suffix;
        suffix *= nums[i];
    }
    return output;
}

int main() {
    vector<int> nums = {1, 2, 3, 4};
    vector<int> result = ProductExceptSelf(nums);
    for (int x : result) cout << x << " ";
    cout << endl;
    return 0;
}