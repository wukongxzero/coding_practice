#include<vector>
#include<iostream>
#include<unordered_map>
using namespace std;

class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node(int _val) : val(_val) {}
};

// DFS + hashmap
// hashmap does two jobs:
// 1. visited check — don't recurse into a node twice (prevents infinite loop)
// 2. clone registry — original→clone, so multiple nodes pointing to the same
//    neighbor all get the same clone back (prevents duplicate nodes)
// register the clone BEFORE recursing into neighbors — so back-edges find it

Node* dfs(Node* node, unordered_map<Node*, Node*>& cloned) {
    if(cloned.count(node)) return cloned[node];

    Node* copy = new Node(node->val);
    cloned[node] = copy;

    for(Node* neighbor : node->neighbors)
        copy->neighbors.push_back(dfs(neighbor, cloned));

    return copy;
}

Node* clone_graph(Node* node) {
    if(!node) return nullptr;
    unordered_map<Node*, Node*> cloned;
    return dfs(node, cloned);
}

int main() {
    // Build: 1 -- 2
    //        |    |
    //        4 -- 3
    Node* n1 = new Node(1);
    Node* n2 = new Node(2);
    Node* n3 = new Node(3);
    Node* n4 = new Node(4);

    n1->neighbors = {n2, n4};
    n2->neighbors = {n1, n3};
    n3->neighbors = {n2, n4};
    n4->neighbors = {n1, n3};

    Node* cloned_n1 = clone_graph(n1);

    cout << "cloned val: " << cloned_n1->val << endl;
    cout << "cloned neighbors: " << cloned_n1->neighbors.size() << endl;
    cout << (n1 != cloned_n1 ? "PASS: different pointers" : "FAIL") << endl;

    return 0;
}
