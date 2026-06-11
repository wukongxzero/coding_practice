#include<unordered_map>
#include<string>


using namespace std;
class Solution {
public:
    bool isAnagram(string s, string t) {
        //check if s and t are equal to each other ?


        if(s.length() != t.length()){
            return false;            
        }

        unordered_map<char,int> count;
        for(char c : s) count[c]++; 

        for(char c : t) count[c]--;
        for(auto& p : count) if(p.second != 0) 
            return false;
         
            return true; 
    }
};


