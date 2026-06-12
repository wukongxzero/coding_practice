//sliding window . Problem best time to buy and sell stock (#121)
//input : prices [7,1,5,3,6,4] output: 5 (buy at 1 ,sell at 6)
//input prices [7,6,4,3,1] output:0 (no profit possible)
// O(n) time, O(1) space , give it a shot in cpp 


#include<vector>
#include<iostream>


using namespace std;

int best_time_to_buy_and_stock(vector<int>& prices){
    
    int min_price = INT_MAX;
    int max_profit = 0;


    for(int price:prices){
        //update min price
        min_price = min(min_price,price);
        max_profit = max(max_profit,price - min_price);
        
        //compute profit 
        //update max_profit 

    }
    return max_profit;

} 