// ArcadiaEngine.cpp - STUDENT TEMPLATE
// TODO: Implement all the functions below according to the assignment requirements

#include "ArcadiaEngine.h"
#include <algorithm>
#include <queue>
#include <numeric>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>

using namespace std;

// =========================================================
// PART A: DATA STRUCTURES (Concrete Implementations)
// =========================================================

// --- 1. PlayerTable (Double Hashing) ---

class ConcretePlayerTable : public PlayerTable {
private:
    // TODO: Define your data structures here
    // Hint: You'll need a hash table with double hashing collision resolution

public:
    ConcretePlayerTable() {
        // TODO: Initialize your hash table
    }

    void insert(int playerID, string name) override {
        // TODO: Implement double hashing insert
        // Remember to handle collisions using h1(key) + i * h2(key)
    }

    string search(int playerID) override {
        // TODO: Implement double hashing search
        // Return "" if player not found
        return "";
    }
};

// --- 2. Leaderboard (Skip List) ---

class ConcreteLeaderboard : public Leaderboard {
private:
    // TODO: Define your skip list node structure and necessary variables
    // Hint: You'll need nodes with multiple forward pointers

public:
    ConcreteLeaderboard() {
        // TODO: Initialize your skip list
    }

    void addScore(int playerID, int score) override {
        // TODO: Implement skip list insertion
        // Remember to maintain descending order by score
    }

    void removePlayer(int playerID) override {
        // TODO: Implement skip list deletion
    }

    vector<int> getTopN(int n) override {
        // TODO: Return top N player IDs in descending score order
        return {};
    }
};

// --- 3. AuctionTree (Red-Black Tree) ---

struct Node {
    int id;
    int price;
    char color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int id, int price, char color = 'r') {
        this->id = id;
        this->price = price;
        this->color = color;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
    ~Node() {
        left = right = parent = nullptr;
    }

    friend ostream& operator<<(ostream& out, const Node& node) {
        return out << node.id << ',' << node.price << ',' << node.color;
    }
    Node& operator=(const Node& other) {
        if (this != &other) {
            id = other.id;
            price = other.price;
            color = other.color;
        }
        return *this;
    }
};

class ConcreteAuctionTree : public AuctionTree {
private:
    Node* root;
    Node* nil;

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nil) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }
    void rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != nil) y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nil) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }
    void insertFixup(Node* x) {
        // parent is red
        while (x != root && x->parent->color == 'r') {
            // parent is left
            if (x->parent == x->parent->parent->left) {
                Node* uncle = x->parent->parent->right;

                // Case 1
                if (uncle->color == 'r') {
                    x->parent->color = 'b';
                    uncle->color = 'b';
                    x->parent->parent->color = 'r';
                    x = x->parent->parent;
                }
                else if (uncle->color == 'b'){
                    // Case 2
                    if (x == x->parent->right) {
                        x = x->parent;
                        rotateLeft(x);
                    }
                    // Case 3
                    x->parent->color = 'b';
                    x->parent->parent->color = 'r';
                    rotateRight(x->parent->parent);
                }
            }
            // parent is right
            else if (x->parent == x->parent->parent->right) {
                Node* uncle = x->parent->parent->left;
                // Case 4
                if (uncle->color == 'r') {
                    x->parent->color = 'b';
                    uncle->color = 'b';
                    x->parent->parent->color = 'r';
                    x = x->parent->parent;
                }
                else if (uncle->color == 'b') {
                    // Case 5
                    if (x == x->parent->left) {
                        x = x->parent;
                        rotateRight(x);
                    }
                    // Case 6
                    x->parent->color = 'b';
                    x->parent->parent->color = 'r';
                    rotateLeft(x->parent->parent);
                }
            }
        }
        root->color = 'b';
    }
    void traveseInorderHelper(Node* x) {
        if (x != nil) {
            traveseInorderHelper(x->left);
            cout << *x << "   ";
            traveseInorderHelper(x->right);
        }
    }
    void traverseInorder() {
        traveseInorderHelper(root);
        cout<<endl;
    }
    void deleteTreeHelper(Node* node) {
        if (node != nil) {
            deleteTreeHelper(node->left);
            deleteTreeHelper(node->right);
            node->left = node->right = nullptr;
            delete node;
        }
    }
    Node* searchItem(int itemID) {
        Node* current = root;
        while (current != nil) {
            if (itemID == current->id) return current;
            else if (itemID < current->id) current = current->left;
            else current = current->right;
        }
        return nil; // Not found
    }
    Node* getPredecessorHelper(Node* node) {
        // get predecessor from left subtree
        Node* current = node->left;
        while (current->right != nil) {
            current = current->right;
        }
        return current;
        
    }

    Node* getSuccessorHelper(Node* node) {
        // get successor from right subtree
        Node* current = node->right;
        while (current->left != nil) {
            current = current->left;
        }
        return current;
    }
    Node* getReplacementNode(Node* node) {
        // get replacement node for deletion
        if (node->left != nil) {
            return getPredecessorHelper(node);
        } else if (node->right != nil) {
            return getSuccessorHelper(node);
        }
        return node; // leaf node
    }
    Node* deleteBSTNode(Node* node) {
        Node* node_child;
        if (node->left != nil) node_child = node->left;
        else node_child = node->right;
        
        // Link child to parent
        node_child->parent = node->parent;

        if (node->parent == nil) root = node_child;
        else if (node == node->parent->left) node->parent->left = node_child;
        else node->parent->right = node_child;
        delete node;
        return node_child;
    }
    void deleteFixup(Node* x) {
        while(x != root && x->color == 'b') { // not case 1
            if (x == x->parent->left) { // x is left child
                Node* sibling = x->parent->right;
                // Case 2: sibling is red
                if (sibling->color == 'r') {
                    sibling->color = 'b';
                    x->parent->color = 'r';
                    rotateLeft(x->parent);
                    sibling = x->parent->right;
                }
                // Case 3: sibling is black, both of sibling's children are black
                else if (sibling->left->color == 'b' && sibling->right->color == 'b') {
                    sibling->color = 'r';
                    x = x->parent;
                }
                else {
                    // Case 4: sibling is black, one of sibling's children is red

                    // sibling's left child is red
                    if (sibling->left->color == 'r') {
                        sibling->left->color = 'b';
                        sibling->color = 'r';
                        rotateRight(sibling);
                        sibling = x->parent->right;
                    }
                    //sibling's right child is red
                    sibling->color = x->parent->color;
                    x->parent->color = 'b';
                    sibling->right->color = 'b';
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            else{ // x is right child
                Node* sibling = x->parent->left;
                // Case 2: sibling is red
                if (sibling->color == 'r') {
                    sibling->color = 'b';
                    x->parent->color = 'r';
                    rotateRight(x->parent);
                    sibling = x->parent->left;
                }
                // Case 3: sibling is black, both of sibling's children are black
                else if (sibling->right->color == 'b' && sibling->left->color == 'b') {
                    sibling->color = 'r';
                    x = x->parent;
                }
                else {
                    // Case 4: sibling is black, one of sibling's children is red

                    // sibling's right child is red
                    if (sibling->right->color == 'r') {
                        sibling->right->color = 'b';
                        sibling->color = 'r';
                        rotateLeft(sibling);
                        sibling = x->parent->left;
                    }
                    //sibling's left child is red
                    sibling->color = x->parent->color;
                    x->parent->color = 'b';
                    sibling->left->color = 'b';
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = 'b';
    }
public:
    ConcreteAuctionTree() {
        root = nil = new Node(0, INT_MAX, 'b');
    }

    void insertItem(int itemID, int price) override {
        Node* new_node = new Node(itemID, price);
        new_node->left = new_node->right = nil;
        Node* parent = nil;
        Node* current = root;

        // Normal BST insertion O(log n)
        while (current != nil) {
            parent = current;
            if (price < current->price) current = current->left;
            else if (price > current->price) current = current->right;
            else if (price == current->price){
                if (itemID < current->id) current = current->left;
                else if (itemID > current->id) current = current->right;
                else {
                    cout << "Price: "<< price << " with ID: "<< itemID << " already exists. (skipped inserting)" << endl;
                    return;
                };
            }
        }

        new_node->parent = parent;

        // Link new_node to parent
        if (parent == nil)
            root = new_node;
        else if (price < parent->price) 
            parent->left = new_node;
        else if (price > parent->price)
            parent->right = new_node;
        else if (price == parent->price)
            if (itemID < parent->id)
                parent->left = new_node;
            else
                parent->right = new_node;

        insertFixup(new_node);

        traverseInorder();
    }

    void deleteItem(int itemID) override {
        Node* node = searchItem(itemID);
        
        if (node == nil) {
            cout << "Item ID: " << itemID << " not found. (skipped deleting)" << endl;
            return;
        }
        Node* replacement_node = getReplacementNode(node);

        // Copy replacement_node data to node
        node->id = replacement_node->id;
        node->price = replacement_node->price;

        char deleted_color = replacement_node->color;
        
        Node* node_child = deleteBSTNode(replacement_node);
        
        // Only need fixup if a black node was deleted
        if (deleted_color == 'b') {
            deleteFixup(node_child);
        }
        
        traverseInorder();
    }
    ~ConcreteAuctionTree() {
        deleteTreeHelper(root);
        delete nil;
        root = nil = nullptr;
    }
};

// =========================================================
// PART B: INVENTORY SYSTEM (Dynamic Programming)
// =========================================================

int InventorySystem::optimizeLootSplit(int n, vector<int>& coins) {
    // TODO: Implement partition problem using DP
    // Goal: Minimize |sum(subset1) - sum(subset2)|
    // Hint: Use subset sum DP to find closest sum to total/2
    return 0;
}

int InventorySystem::maximizeCarryValue(int capacity, vector<pair<int, int>>& items) {
    // TODO: Implement 0/1 Knapsack using DP
    // items = {weight, value} pairs
    // Return maximum value achievable within capacity
    return 0;
}

long long InventorySystem::countStringPossibilities(string s) {
    // TODO: Implement string decoding DP
    // Rules: "uu" can be decoded as "w" or "uu"
    //        "nn" can be decoded as "m" or "nn"
    // Count total possible decodings
    return 0;
}

// =========================================================
// PART C: WORLD NAVIGATOR (Graphs)
// =========================================================

bool WorldNavigator::pathExists(int n, vector<vector<int>>& edges, int source, int dest) {
    // time: O(V + E), space: O(V + E)
    if (source == dest){
        return true;
    }
    // Build adjacency list
    vector<vector<int>> adj(n);
    for (auto& edge : edges) {
        int u = edge[0], v = edge[1];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    // Push source into queue
    queue<int> q;
    q.push(source);
    // Mark it visited
    vector<bool> visited(n, false);
    visited[source] = true;
    // While queue not empty:
    while (!q.empty()) {
        // Pop node
        int node = q.front();
        q.pop();
        // If node == dest → return true
        if (node == dest) {
            return true;
        }
        // Push unvisited neighbors
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    // If finished → return false
    return false;
}

long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate, vector<vector<int>>& roadData) {
    // TODO: Implement Minimum Spanning Tree (Kruskal's or Prim's)
    // roadData[i] = {u, v, goldCost, silverCost}
    // Total cost = goldCost * goldRate + silverCost * silverRate
    // Return -1 if graph cannot be fully connected
    return -1;
}

string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads) {
    // TODO: Implement All-Pairs Shortest Path (Floyd-Warshall)
    // Sum all shortest distances between unique pairs (i < j)
    // Return the sum as a binary string
    // Hint: Handle large numbers carefully
    return "0";
}

// =========================================================
// PART D: SERVER KERNEL (Greedy)
// =========================================================

int ServerKernel::minIntervals(vector<char>& tasks, int n) {
    // TODO: Implement task scheduler with cooling time
    // Same task must wait 'n' intervals before running again
    // Return minimum total intervals needed (including idle time)
    // Hint: Use greedy approach with frequency counting
    return 0;
}

// =========================================================
// FACTORY FUNCTIONS (Required for Testing)
// =========================================================

extern "C" {
    PlayerTable* createPlayerTable() { 
        return new ConcretePlayerTable(); 
    }

    Leaderboard* createLeaderboard() { 
        return new ConcreteLeaderboard(); 
    }

    AuctionTree* createAuctionTree() { 
        return new ConcreteAuctionTree(); 
    }
}