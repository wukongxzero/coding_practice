#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int calculate_max_water(vector<int>& Height){
    int left = 0;
    int right = Height.size() - 1;
    int max_area = 0;
    while (left < right){
        int area = min(Height[left], Height[right]) * (right - left);
        max_area = max(max_area, area);
        if (Height[left] < Height[right])
            left++;
        else
            right--;
    }
    return max_area;
}

int main(){
    vector<int> h1 = {1,8,6,2,5,4,8,3,7};
    vector<int> h2 = {1,1};
    cout << calculate_max_water(h1) << endl; // 49
    cout << calculate_max_water(h2) << endl; // 1
}