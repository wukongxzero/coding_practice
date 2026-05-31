#include<stdio.h>
#include<iostream.h>
#include<algorithm.h>
#include<cmath>

using namespace std;
class Solution {
public:
    bool isAnagram(string s, string t) {
        //check if s and t are equal to each other ?


        if(s.length() != t.length()){
            return false;            
        }

        sort(s.begin(),s.end());
        sort(t.begin(),t.end());

        return s == t;

    }
};


