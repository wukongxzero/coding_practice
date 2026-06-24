//there are n courses labeled 0 and n-1 , you are given a list of prerequisites where prerequisites[i] = [a,b] means you must take course b before course a. return True if you can finish all courses , false if not
#include<vector>
#include<queue>
using namespace std;
bool can_finish(int numCourses, vector<vector<int>>& prerequisites){
    vector<vector<int>> adj(numCourses);
    vector<int> indegree(numCourses,0);

    for(auto& p : prerequisites){
        int a = p[0];
        int b = p[1];
        adj[b].push_back(a);
        indegree[a]++;
    }

    queue<int> q;

    for (int i = 0;i<numCourses;i++){
        if(indegree[i] == 0)
            q.push(i);
    }

    int taken = 0;

    while(!q.empty()){
        int node = q.front();
        q.pop();
        taken++;

        for(int nei: adj[node]){
            indegree[nei]--;
            if(indegree[nei] == 0){
                q.push(nei);
            }
        }
    } 
    return taken == numCourses;
}