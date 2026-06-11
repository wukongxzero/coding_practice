#include<vector>
#include<iostream>
#include<unordered_set>

//time complexity o(81)-fixed 9x9 grid :
//space complexity  o(81)=o(1)-27 sets max 9 chars each 
bool isValidSudoku(vector<vector<char>>& board){
    //need 3 arrays of size 9 
    unordered_set<char> rows[9],cols[9],boxes[9];

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board[i][j]!='.'){
                char c = board[i][j];
                if (rows[i].count(c)) return false;
                rows[i].insert(c);
                if(cols[j].count(c)) return false;
                cols[j].insert(c);
                int box_id = (i/3)*3 +(j/3);
                if(boxes[box_id].count(c)) return false;
                boxes[box_id].insert(c);

            }
        }
    }

    return true;

}