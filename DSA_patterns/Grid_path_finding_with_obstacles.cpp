#include<iostream>
#include<vector>
#include<tuple>
#include<unordered_map>
#include<queue>
#include<climits>

using namespace std;
int heuristic(int x, int y, int gx, int gy) {
    // Manhattan distance
    return abs(x - gx) + abs(y - gy);
}
int astar(vector<vector<int>>&grid, pair<int,int> start,pair<int,int> goal){
    int ROWS = grid.size(), COLS = grid[0].size();
    auto [sr,sc] = start;
    auto [gr,gc] = goal;

    //auto h = [&](int r, int c) {return abs(r - gr) + abs(c - gc) ; };
    int h = heuristic(sr,sc,gr,gc);

    //Put start into a priority queue.
    //Pick the node with smallest f.
    //If it's the goal → done.
    //Visit neighbors.
    //Update costs if a better path is found.
    //Repeat.

    //this puts up f,g,row,col
    //using T = tuple<int,int,int,int>;
    //priority_queue<T, vector<T>, greater<T>> pq;
    priority_queue<
        tuple<int,int,int,int>,
        vector<tuple<int,int,int,int>>,
        greater<>
    > pq;
    vector<vector<int>> g(ROWS,vector<int> (COLS,INT_MAX));
    
    g[sr][sc] = 0;
    //pq.push({h(sr,sc),0,(sr,sc)});
    pq.push(make_tuple(h,0,sr,sc));
    //create directions 
    int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

    //while q is not empty 
    while(!pq.empty()){
        auto [f,cost,r,c] = pq.top();
        pq.pop();
        if(r == gr && c == gc){
            return cost;
        }
        
        if(cost>g[r][c]) continue; //non minimum values 

        for(auto& d:dirs){
            int nr = r + d[0] ,nc = c + d[1];
            if(nr >= 0 && nr<ROWS && nc>=0 && nc<COLS && g[nr][nc] !=1){
                int new_g = cost + 1;

                if(new_g<g[nr][nc]){
                    g[nr][nc] = new_g;
                    pq.push(make_tuple(new_g + heuristic(nr,nc,gr,gc), new_g, nr,nc));
                }
            }
        }


    }
    return -1; 


}

int main(){

    vector<vector<int>> grid = {
        {0,0,1,0},
        {0,0,0,1},
        {1,0,0,0},
        {0,1,0,0}
    };  
    int result = astar(grid,{0,0},{3,3});
    cout<<"shortest path: "<< result <<endl;
    return 0; 
}