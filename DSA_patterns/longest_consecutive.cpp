#include<vector>
#include<unordered_set>
#include<iostream>
using namespace std;

int longestConsecutive(vector<int>&nums){
    unordered_set<int> numSet(nums.begin(),nums.end());
    int longest = 0;

    for(int n:nums){
        if(numSet.find(n-1)==numSet.end()){
            int length = 1;
            while(numSet.find(n+length) != numSet.end()){
                length++;
            }
            longest = max(longest,length);
        }
    }
    return longest;
}

int main(){
    vector<int> a = {100,4,200,1,3,2};
    vector<int> b = {0,3,7,2,5,8,4,6,0,1};
    cout << longestConsecutive(a) << endl; // expect 4
    cout << longestConsecutive(b) << endl; // expect 9
}
