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
    struct Entry{
        int playerId = -1;
        string name = "";
    };


    Entry arr[101];
    int count;

public:
    ConcretePlayerTable() {
        // TODO: Initialize your hash table
        this->count = 0;
    }

    void insert(int playerID, string name) override {
        // TODO: Implement double hashing insert
        // Remember to handle collisions using h1(key) + i * h2(key)
        int index;
        if (count == 101){
            cout << "Table is full" << endl;
            return;
        }
        for (int i = 0; i < 100; ++i) {
            index = ((playerID%101) + i * (1 + playerID%100))%101;
            if(arr[index].playerId == -1){
                arr[index].playerId = playerID;
                arr[index].name = name;
                this->count++;
                break;
            }
        }
    }

    string search(int playerID) override {
        // TODO: Implement double hashing search
        // Return "" if player not found
        for (int i = 0; i < 101; i++) {
            int index = ((playerID%101) + i * (1 + playerID%100))%101;

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

class ConcreteAuctionTree : public AuctionTree {
private:
    // TODO: Define your Red-Black Tree node structure
    // Hint: Each node needs: id, price, color, left, right, parent pointers

public:
    ConcreteAuctionTree() {
        // TODO: Initialize your Red-Black Tree
    }

    void insertItem(int itemID, int price) override {
        // TODO: Implement Red-Black Tree insertion
        // Remember to maintain RB-Tree properties with rotations and recoloring
    }

    void deleteItem(int itemID) override {
        // TODO: Implement Red-Black Tree deletion
        // This is complex - handle all cases carefully
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
    // TODO: Implement string decoding DP
    // Complexity: O(n) time, O(n) space where n = length of string
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
    for (int i = 0; i < edges.size(); i++) {
        int u = edges[i][0], v = edges[i][1];
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
    // roadData[i] = {u, v, goldCost, silverCost}
    // n : number of cities
    // m : number of roads
    // u : start city
    // v : end city
    // Total cost = goldCost * goldRate + silverCost * silverRate
    // Build adjacency list
    // time: O(E log V), space: O(V + E)
    vector<vector<tuple<int, long long>>> adj(n);
    for (const auto& road : roadData) {
        int u = road[0];
        int v = road[1];
        long long cost = road[2] * goldRate + road[3] * silverRate;
        adj[u].push_back({v, cost});
        adj[v].push_back({u, cost});
    }
    vector<long long> key(n, LLONG_MAX);   // key[v] ← ∞
    vector<bool> inMST(n, false);           // v ∈ Q ?
    vector<int> parent(n, -1);              // π[v]

    // Q ← V  (min-heap ordered by key)
    priority_queue<tuple<long long, int>,vector<tuple<long long, int>>,greater<>> Q;

    // key[s] ← 0 (start from vertex 0)
    key[0] = 0;
    Q.push({0, 0});

    long long totalCost = 0;
    int connected = 0;

    // while Q ≠ ∅
    while (!Q.empty()) {
        auto top = Q.top();
        long long curKey = get<0>(top);
        int u = get<1>(top);
        Q.pop();

        // Ignore outdated entries
        if (inMST[u]) continue;

        // u ← EXTRACT-MIN(Q)
        inMST[u] = true;
        totalCost += curKey;
        connected++;

        // for each v ∈ Adj[u]
        for (auto& edge : adj[u]) {
            int v = get<0>(edge);
            long long w = get<1>(edge);
            // if v ∈ Q and w(u, v) < key[v]
            if (!inMST[v] && w < key[v]) {
                key[v] = w;        // DECREASE-KEY
                parent[v] = u;     // π[v] ← u
                Q.push({key[v], v});
            }
        }
    }

    // Connectivity check
    if (connected != n) {
        return -1;
    }
    return totalCost;
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

    vector<int> freq(26, 0);
    for (char t : tasks) {
        freq[t - 'A']++;
    }

    // Find maximum frequency
    int f_max = *max_element(freq.begin(), freq.end());

    // Count how many tasks have this maximum frequency
    int count_max = 0;
    for (int f : freq) {
        if (f == f_max)
            count_max++;
    }

    int totalTasks = tasks.size();

    // Greedy formula
    int result = max(
            totalTasks,
            (f_max - 1) * (n + 1) + count_max
    );

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