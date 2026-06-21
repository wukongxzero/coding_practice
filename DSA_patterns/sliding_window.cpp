#include<string> 
#include<vector>
#include<unordered_map>
// the naive approach : check every possible substring . which is O(n_square)--> too slow 
// the sliding window approach --> two pointer approach --> left and right , expand right to pull in characters , when you have all of A,B,C inside the window shrink from left to find the smallest valid window.
// how do you know when you have all of A,B,C ? so required = 3 you need 3 distinct characters satisfied . have starts at 0 and increments each time one character becomes fully covered 

//when have == required (==3 ), the window is valid !
// why window[c] == need[c] exactly (not>=) ? because you only want to count the moment a character becomes newly satisfied --> not every time you add another copy of it . if t = AA , you only need 2 A's . adding the first A gets you window[A] = 1, need[A]=2 --> not satisfied . adding the second A gets you window[A] = 2 ==need[A] =2 . now increment havae . adding a third A : window[A] =3 -> need[A]=2 - already satisfied, dont double count.

string minWindow(string s, string t){
    unordered_map<char,int> need,window;
    for(char c:t) need[c]++;

    int have = 0, required = need.size();
    int left = 0, min_len = INT_MAX, min_left = 0;


    for (int right = 0;right<s.size();right++){
        char c = s[right];
        window[c]++;
        if(need.count(c) && window[c]==need[c])
            have++;

        while (have == required) { 
            if(right - left + 1< min_len){
                min_len = right - left + 1 ;
                min_left = left;

            }
            char lc = s[left];
            window[lc] --;
            if(need.count(lc) && window[lc]< need[lc])
                have--;
            left++;
        }
    }

    return min_len == INT_MAX? "" : s.substr(min_left,min_len);

}

// to remember or understand :
//1. need = what t requires . window = what the current window has.
//2. have counts how many distinct characters are fully satisfied (window[c] == need[c]). hwne have == required , the window is valid.
//3. the inner while shrinks from the left as long as the window statys valid -- record the minimum each time.
