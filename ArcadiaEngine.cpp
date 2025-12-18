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
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <bitset>

using namespace std;

// =========================================================
// PART A: DATA STRUCTURES (Concrete Implementations)
// =========================================================

// --- 1. PlayerTable (Double Hashing) ---

class ConcretePlayerTable : public PlayerTable {
private:
    // Define your data structures here
    // Hint: You'll need a hash table with double hashing collision resolution
    struct Entry{
        int playerId = -1;
        string name = "";
    };
    Entry arr[101];
    int count;

public:
    ConcretePlayerTable() {
        // Initialize your hash table
        this->count = 0;
    }

    void insert(int playerID, string name) override {
        // Implement double hashing insert
        // Remember to handle collisions using h1(key) + i * h2(key)
        // time complexity: O(1) on average, O(n) in worst case
        int index;

        if (count == 101){
            cout << "Table is Full" << endl;
            return;
        }
        for (int i = 0; i < 101; ++i) {
            index = ((playerID%101) + i * (97 - playerID%97))%101;
            if(arr[index].playerId == -1){
                arr[index].playerId = playerID;
                arr[index].name = name;
                this->count++;
                return;
            }
            if (arr[index].playerId == playerID) {
                arr[index].name = name; // update
                return;
            }
        }
        cout << "Table is Full" << endl;
    }
    string search(int playerID) override {
        // Implement double hashing search
        // Return "" if player not found
        // time complexity: O(1) on average, O(n) in worst case
        for (int i = 0; i < 101; i++) {
            int index = ((playerID%101) + i * (97 - playerID%97))%101;
            // Stop early if slot is empty
            if (arr[index].playerId == -1) {
                return "";
            }
            // Found the player
            if (arr[index].playerId == playerID) {
                return arr[index].name;
            }
        }
        return "";
    }
};

// --- 2. Leaderboard (Skip List) ---

class ConcreteLeaderboard : public Leaderboard {
private:
    struct SkipNode {
        int playerID;
        int score;
        int level;
        vector<SkipNode*> forward;
        SkipNode(int id, int s, int lvl) : playerID(id), score(s), level(lvl) {
            forward.resize(lvl + 1, nullptr);
        }
    };
    SkipNode* head;
    int maxLevel;
    float probability;
    int currentLevel;
    // Helper: Coin flip to determine node level
    int randomLevel() const {
        int lvl = 0;
        while (((float)rand() / RAND_MAX) < probability && lvl < maxLevel) {
            lvl++;
        }
        return lvl;
    }
    static bool isHigherRank(int scoreA, int idA, int scoreB, int idB) {
        if (scoreA > scoreB) return true;
        if (scoreA == scoreB && idA < idB) return true;
        return false;
    }

public:
    ConcreteLeaderboard() {
        maxLevel = 16;
        probability = 0.5f;
        currentLevel = 0;
        head = new SkipNode(-1, INT_MAX, maxLevel);
        srand(time(nullptr));
    }
    ~ConcreteLeaderboard() {
        SkipNode* curr = head;
        while (curr) {
            SkipNode* next = curr->forward[0];
            delete curr;
            curr = next;
        }
    }
    void addScore(int playerID, int score) override {
        vector<SkipNode*> update(maxLevel + 1);
        SkipNode* curr = head;
        // 1. Find insert position (O(log n))
        for (int i = currentLevel; i >= 0; i--) {
            while (curr->forward[i] != nullptr && isHigherRank(curr->forward[i]->score, curr->forward[i]->playerID, score, playerID)) {
                curr = curr->forward[i];
            }
            update[i] = curr;
        }
        // 2. Generate random level
        int newLevel = randomLevel();
        if (newLevel > currentLevel) {
            for (int i = currentLevel + 1; i <= newLevel; i++) {
                update[i] = head;
            }
            currentLevel = newLevel;
        }
        // 3. Insert Node
        auto* newNode = new SkipNode(playerID, score, newLevel);
        for (int i = 0; i <= newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }
    void removePlayer(int playerID) override {
        SkipNode* target = nullptr;
        SkipNode* scan = head->forward[0];
        // 1. Linear Scan to find target score (O(N))
        while (scan != nullptr) {
            if (scan->playerID == playerID) {
                target = scan;
                break;
            }
            scan = scan->forward[0];
        }
        if (!target) {
            return; // Player not found
        }
        // 2. Standard Skip List Delete (O(log n)) using the score we just found
        vector<SkipNode*> update(maxLevel + 1);
        SkipNode* curr = head;
        int targetScore = target->score;
        int targetID = target->playerID;
        for (int i = currentLevel; i >= 0; i--) {
            while (curr->forward[i] != nullptr && isHigherRank(curr->forward[i]->score, curr->forward[i]->playerID, targetScore, targetID)) {
                curr = curr->forward[i];
            }
            update[i] = curr;
        }
        // 3. Unlink the node
        // After traversal, update[0]->forward[0] should point to target (or a node with same rank)
        SkipNode* nodeToDelete = update[0]->forward[0];
        // Verify this is actually our target node
        if (nodeToDelete != nullptr && nodeToDelete == target) {
            for (int i = 0; i <= currentLevel; i++) {
                if (update[i]->forward[i] != target) break;
                update[i]->forward[i] = target->forward[i];
            }
            delete target;
            // Reduce level if needed
            while (currentLevel > 0 && head->forward[currentLevel] == nullptr) {
                currentLevel--;
            }
        }
    }
    vector<int> getTopN(int n) override {
        vector<int> results;
        SkipNode* curr = head->forward[0];

        // Simple traversal of the sorted list
        while (curr != nullptr && results.size() < n) {
            results.push_back(curr->playerID);
            curr = curr->forward[0];
        }
        return results;
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

    friend ostream& operator<<(ostream& out, const Node& node) {
        return out << node.id << ',' << node.price << ',' << node.color;
    }
    Node& operator=(const Node& other) {
        if (this != &other) {
            id = other.id;
            price = other.price;
            color = other.color;
            left = other.left;
            right = other.right;
            parent = other.parent;
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
    void traverseInorderHelper(Node* x) {
        if (x != nil) {
            traverseInorderHelper(x->left);
            cout << *x << "   ";
            traverseInorderHelper(x->right);
        }
    }
    void traverseInorder() {
        traverseInorderHelper(root);
        cout<<endl;
    }
    void deleteTreeHelper(Node* node) {
        if (node != nil) {
            deleteTreeHelper(node->left);
            deleteTreeHelper(node->right);
            delete node;
        }
    }
    Node* searchItemHelper(Node* node, int itemID) {
        if (node == nil) {
            return nil;
        }
        if (node->id == itemID) {
            return node;
        }
        // Search left subtree
        Node* found = searchItemHelper(node->left, itemID);
        if (found != nil) {
            return found;
        }
        // Search right subtree
        return searchItemHelper(node->right, itemID);
    }
    Node* searchItem(int itemID) {
        return searchItemHelper(root, itemID);
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
        nil = new Node(0, INT_MAX, 'b');
        root = nil;
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
                    delete new_node;
                    return;
                };
            }
        }

        new_node->parent = parent;

        // Link new_node to parent
        if (parent == nil) {
            root = new_node;
        }
        else if (price < parent->price || (price == parent->price && itemID < parent->id)) {
            parent->left = new_node;
        }
        else {
            parent->right = new_node;
        }

        // O(log n)
        insertFixup(new_node);

        // O(n)
        traverseInorder();
    }

    void deleteItem(int itemID) override {
        // O(n)
        Node* node = searchItem(itemID);
        
        if (node == nil) {
            cout << "Item ID: " << itemID << " not found. (skipped deleting)" << endl;
            return;
        }
        // O(log n)
        Node* replacement_node = getReplacementNode(node);

        // Copy replacement_node data to node
        node->id = replacement_node->id;
        node->price = replacement_node->price;

        char deleted_color = replacement_node->color;
        
        // O(log n)
        Node* node_child = deleteBSTNode(replacement_node);
        
        // Only need fixup if a black node was deleted
        if (deleted_color == 'b') {
            // O(log n)
            deleteFixup(node_child);
        }

        nil->parent = nullptr;
        
        // O(n)
        traverseInorder();
    }
    ~ConcreteAuctionTree() {
        if (root != nil) {
            deleteTreeHelper(root);
        }
        delete nil;
    }
};

// =========================================================
// PART B: INVENTORY SYSTEM (Dynamic Programming)
// =========================================================


//i often wonder if life is worth living
//i often wonder if life is worth living
//i often wonder if life is worth living
int InventorySystem::optimizeLootSplit(int n, vector<int>& coins) {
    // Complexity: O(n × sum) time, O(sum) space
    // where sum = total sum of all coins
    
    int total = 0;
    for (int coin : coins) {
        total += coin;
    }
    
    int target = total / 2;
    
    vector<bool> dp(target + 1, false);
    dp[0] = true;
    
    for (int coin : coins) {
        for (int j = target; j >= coin; j--) {
            if (dp[j - coin]) {
                dp[j] = true;
            }
        }
    }
    
    int maxSum = 0;
    for (int j = target; j >= 0; j--) {
        if (dp[j]) {
            maxSum = j;
            break;
        }
    }

    return total - 2 * maxSum;
}

int InventorySystem::maximizeCarryValue(int capacity, vector<pair<int, int>>& items) {
    // Complexity: O(n × W) time, O(W) space
    // where n = number of items, W = capacity
    vector<int> dp(capacity + 1, 0);

    for (auto& item : items) {
        int weight = item.first;
        int value = item.second;
        
        // For each possible weight w (from capacity down to weight)
        // If we can carry (w - weight), we can now carry w with this item
        for (int w = capacity; w >= weight; w--) {
            dp[w] = max(dp[w], dp[w - weight] + value);
        }
    }
    
    // Return maximum value achievable within capacity
    return dp[capacity];
}

long long InventorySystem::countStringPossibilities(string s) {
    // Complexity: O(n) time, O(1) space
    // Rules: "uu" can be decoded as "w" or "uu"
    //        "nn" can be decoded as "m" or "nn"
    // Count total possible decodings
    const long long MOD = 1e9 + 7;
    // Empty string:  1 way
    if (s.empty()) {
        return 1;
    }
    int n = s.length();
    long long prev2 = 1;  // dp[0]
    long long prev1 = 1;  // dp[1]
    long long curr = 1;
    // Check for invalid characters at start
    if (s[0] == 'm' || s[0] == 'w') {
        return 0;
    }
    for (int i = 2; i <= n; i++) {
        // Check for invalid characters
        if (s[i-1] == 'm' || s[i-1] == 'w') {
            return 0;
        }
        // Always can keep current character as-is
        curr = prev1;
        // Check if we can merge previous two characters
        if ((s[i-2] == 'u' && s[i-1] == 'u') || (s[i-2] == 'n' && s[i-1] == 'n')) {
            curr = (curr + prev2) % MOD;
        }
        // Shift values
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

// =========================================================
// PART C: WORLD NAVIGATOR (Graphs)
// =========================================================

bool WorldNavigator::pathExists(int n, vector<vector<int>>& edges, int source, int dest) {
    // time: O(V + E)
    if (source == dest){
        return true;
    }
    
    // Create mapping from actual node IDs to normalized [0, n-1]
    unordered_map<int, int> nodeToIdx;
    int idx = 0;
    
    // Collect all unique nodes
    unordered_set<int> nodes;
    nodes.insert(source);
    nodes.insert(dest);
    for (const auto& edge : edges) {
        nodes.insert(edge[0]);
        nodes.insert(edge[1]);
    }
    
    // Assign indices
    for (int node : nodes) {
        nodeToIdx[node] = idx++;
    }
    
    // Build adjacency list with normalized indices
    vector<vector<int>> adj(nodes.size());
    for (const auto& edge : edges) {
        int u = nodeToIdx[edge[0]], v = nodeToIdx[edge[1]];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    int normSource = nodeToIdx[source];
    int normDest = nodeToIdx[dest];
    
    // Push source into queue
    queue<int> q;
    q.push(normSource);
    // Mark it visited
    vector<bool> visited(nodes.size(), false);
    visited[normSource] = true;
    // While queue not empty:
    while (!q.empty()) {
        // Pop node
        int node = q.front();
        q.pop();
        // If node == dest -> return true
        if (node == normDest) {
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
    // If finished -> return false
    return false;
}

long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate, vector<vector<int>>& roadData) {
    // time: O(E log V)
        // roadData[i] = {u, v, goldCost, silverCost}
        // n : number of cities
        // m : number of roads
        // u : start city
        // v : end city
        // Total cost = goldCost * goldRate + silverCost * silverRate
    // Create mapping from actual node IDs to normalized [0, n-1]
    unordered_map<int, int> nodeToIdx;
    unordered_set<int> nodes;
    
    // Collect all unique nodes
    for (const auto& road : roadData) {
        nodes.insert(road[0]);
        nodes.insert(road[1]);
    }
    
    // Assign indices
    int idx = 0;
    for (int node : nodes) {
        nodeToIdx[node] = idx++;
    }
    
    int actualN = nodes.size();
    
    // Build adjacency list with normalized indices
    vector<vector<tuple<int, long long>>> adj(actualN);
    for (const auto& road : roadData) {
        int u = nodeToIdx[road[0]];
        int v = nodeToIdx[road[1]];
        long long cost = road[2] * goldRate + road[3] * silverRate;
        adj[u].push_back({v, cost});
        adj[v].push_back({u, cost});
    }
    vector<long long> key(actualN, LLONG_MAX);   // key[v] = infinity
    vector<bool> inMST(actualN, false);           // v belong to Q ?
    vector<int> parent(actualN, -1);              // π[v]

    // Q = V  (min-heap ordered by key)
    priority_queue<tuple<long long, int>,vector<tuple<long long, int>>,greater<>> Q;

    // key[s] = 0 (start from vertex 0)
    key[0] = 0;
    Q.push({0, 0});

    long long totalCost = 0;
    int connected = 0;

    // while Q not empty
    while (!Q.empty()) {
        auto top = Q.top();
        long long curKey = get<0>(top);
        int u = get<1>(top);
        Q.pop();

        // ignore outdated entries
        if (inMST[u]) {
            continue;
        }

        // u = EXTRACT-MIN(Q)
        inMST[u] = true;
        totalCost += curKey;
        connected++;

        // for each v ∈ Adj[u]
        for (auto& edge : adj[u]) {
            int v = get<0>(edge);
            long long w = get<1>(edge);
            // if v belong to Q and w < key[v]
            if (!inMST[v] && w < key[v]) {
                key[v] = w;        // DECREASE-KEY
                parent[v] = u;     // π[v] = u
                Q.push({key[v], v});
            }
        }
    }

    // Connectivity check
    if (connected != n) {
        return -1;
    }
    // return total cost of MST
    return totalCost;
}

string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads) {
    // time: O(V^3)
    // Implement All-Pairs Shortest Path (Floyd-Warshall)
    // Sum all shortest distances between unique pairs (i < j)
    // Return the sum as a binary string
    // Hint: Handle large numbers carefully
    // undirected graph
    // input roads[i] = {u, v, weight}
    // formula: Ak[i][j] = min(A(k-1)[i][j], A(k-1)[i][k] + A(k-1)[k][j])
    const long long INF = 1e18;

    // Map arbitrary node IDs to compact indices
    unordered_map<int, int> nodeToIdx;
    int nodeCount = 0;
    
    for (const auto& road : roads) {
        if (nodeToIdx.find(road[0]) == nodeToIdx.end()) {
            nodeToIdx[road[0]] = nodeCount++;
        }
        if (nodeToIdx.find(road[1]) == nodeToIdx.end()) {
            nodeToIdx[road[1]] = nodeCount++;
        }
    }
    
    if (nodeCount == 0) return "0";
    
    // Initialize distance matrix
    vector<vector<long long>> dist(nodeCount, vector<long long>(nodeCount, INF));
    for (int i = 0; i < nodeCount; i++) {
        dist[i][i] = 0;
    }
    
    // Add edges (undirected)
    for (const auto& road : roads) {
        int u = nodeToIdx[road[0]];
        int v = nodeToIdx[road[1]];
        long long w = road[2];
        dist[u][v] = min(dist[u][v], w);
        dist[v][u] = min(dist[v][u], w);
    }
    
    // Floyd-Warshall
    for (int k = 0; k < nodeCount; k++) {
        for (int i = 0; i < nodeCount; i++) {
            for (int j = 0; j < nodeCount; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    
    // Sum distances for unique pairs (i < j) using __int128
    __int128 total = 0;
    for (int i = 0; i < nodeCount; i++) {
        for (int j = i + 1; j < nodeCount; j++) {
            if (dist[i][j] != INF) {
                total += dist[i][j];
            }
        }
    }
    
    // Convert to binary string
    if (total == 0) {
        return "0";
    }
    
    string binary;
    while (total > 0) {
        // if least significant bit is 1
        if (total & 1) {
            binary.push_back('1');
        } else {
            binary.push_back('0');
        }
        total >>= 1;
    }
    reverse(binary.begin(), binary.end());
    
    return binary;
}

// =========================================================
// PART D: SERVER KERNEL (Greedy)
// =========================================================

int ServerKernel::minIntervals(vector<char>& tasks, int n) {
    // TODO: Implement task scheduler with cooling time
    // Same task must wait 'n' intervals before running again
    // Return minimum total intervals needed (including idle time)
    // Hint: Use greedy approach with frequency counting

    vector<int> freq(26, 0);
    for (char t : tasks) {
        freq[t - 'A']++;
    }

    // Find maximum frequency
    int f_max = *max_element(freq.begin(), freq.end());

    // Count how many tasks have this maximum frequency
    int count_max = 0;
    for (int f : freq) {
        if (f == f_max) {
            count_max++;
        }
    }

    int totalTasks = tasks.size();

    // Greedy formula
    int result = max(totalTasks,(f_max - 1) * (n + 1) + count_max);

    return result;

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