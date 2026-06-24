// 
#include<vector>
#include<iostream>

using namespace std;

int calculate_water_trapped(vector<int>& Height){
    int left = 0;
    int right = Height.size() - 1 ;
    int left_max = 0;
    int right_max = 0;
    int water = 0;

    while(left<right){
        left_max = max(left_max,Height[left]);
        right_max = max(right_max,Height[right]);

        if(Height[left]<Height[right]){
            water += left_max - Height[left];
            left++;
        }
        else 
        {
            water += right_max - Height[right];
            right--;
        }
    }
    return water;


    
}

int main(){
    vector<int> h1 = {0,1,0,2,1,0,1,3,2,1,2,1};
    vector<int> h2 = {4,2,0,3,2,5};
    cout<<calculate_water_trapped(h1)<<endl;
    cout<<calculate_water_trapped(h2)<<endl;
    return 0; 
}