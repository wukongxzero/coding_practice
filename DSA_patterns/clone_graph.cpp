#include<vector>
#include<iostream>
#include<unordered_map>
using namespace std;

class Node{
    public:
    int val;
    vector<Node*> neighbors;
    Node(int _val) : val(_val){}
};

// why DFS and not some random data structure? --> infinite loop if you revisit the same node
// if node 1 and node 2 both have each other as neighbors , a plain DFS would create duplicate clones of the same node. you would end up with multiple copies of node 1 instead of one.


//we use a hashmap here which does two jobs:

// visited check - dont recurse into a node twice 
// clone registry -original-> clone , so when multiple nodes point to the same neighbour , they all get the same clone back 



//clone node:
//if node in cloned --> return cloned[node]
//create new node 
//add to cloned 
//for each neighbor --> recursively clone and add to new node's neighbors 
//return new node 

Node* dfs(Node* node ,unordered_map<Node*,Node*>& cloned){
    if(cloned.count(node)) return cloned[node];
    Node* copy = new Node(node->val);
    cloned[node] = copy;
    //register before recursing -- so if a neighbor points back to this node , it finds it in the map instead of looping forever
    for(Node* neighbor : node-> neighbors){
        copy->neighbors.push_back(dfs(neighbor,cloned));
        //this says: for every neighbor of the original , recursively clone it and add the clone to our copy's neighbor list.
        
    }
    return copy;
}

Node* clone_graph(Node* node){
    unordered_map<Node*,Node*> cloned;
    if(!node) return nullptr;
    return dfs(node,cloned);
}