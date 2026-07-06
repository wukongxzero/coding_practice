// Course Schedule (#207) — Kahn's algorithm (BFS topological sort)
// Build adj list + indegree array. BFS from all indegree-0 nodes.
// If taken == numCourses, no cycle exists → can finish all courses.


#include<vector>
#include<queue> 

using namespace std;
bool can_finish(int num_course, vector<vector<int>>& prerequisites){
    vector<vector<int>> adj(num_course);

    vector<int> indegree(num_course,0);

    for(auto& p: prerequisites){
        int a = p[0];
        int b = p[1];

        adj[b].push_back(a);
        indegree[a]++;
    }

    queue<int> q;
    for (int i =0;i<num_course;i++){
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
    return taken == num_course; 
}