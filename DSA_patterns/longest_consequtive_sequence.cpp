#include<iostream>
#include<vector>
#include<unordered_set>

using namespace std;

int longest_consecutive_array(vector<int>& nums){
    //1.) load nums into an unordered_set

    unordered_set<int> s(nums.begin(),nums.end());

    int longest = 0;

    for(int num:s){
        if(s.count(num - 1)==0){
            int length = 1;
            while(s.count(num + length) != 0){
                length++;

            }

            longest = max(longest,length);
        }
    }

    return longest;
    //2.)iterate only start counting when num-1 is not in the set 
    //3.)count up while consecutive numbers exist.

}
int main(){
    vector<int> nums = {100,4,200,1,3,2};
    cout <<longest_consecutive_array(nums) << endl;
    return 0;

}