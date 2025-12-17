/**
 * main_test_student.cpp
 * Basic "Happy Path" Test Suite for ArcadiaEngine
 * Use this to verify your basic logic against the assignment examples.
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <functional>
#include "ArcadiaEngine.h" 

using namespace std;

// ==========================================
// FACTORY FUNCTIONS (LINKING)
// ==========================================
// These link to the functions at the bottom of your .cpp file
extern "C" {
    PlayerTable* createPlayerTable();
    Leaderboard* createLeaderboard();
    AuctionTree* createAuctionTree();
}

// ==========================================
// TEST UTILITIES
// ==========================================
class StudentTestRunner {
	int count = 0;
    int passed = 0;
    int failed = 0;

public:
    void runTest(string testName, bool condition) {
		count++;
        cout << "TEST: " << left << setw(50) << testName;
        if (condition) {
            cout << "[ PASS ]";
            passed++;
        } else {
            cout << "[ FAIL ]";
            failed++;
        }
        cout << endl;
    }

    void printSummary() {
        cout << "\n==========================================" << endl;
        cout << "SUMMARY: Passed: " << passed << " | Failed: " << failed << endl;
        cout << "==========================================" << endl;
		cout << "TOTAL TESTS: " << count << endl;
        if (failed == 0) {
            cout << "Great job! All basic scenarios passed." << endl;
            cout << "Now make sure to handle edge cases (empty inputs, collisions, etc.)!" << endl;
        } else {
            cout << "Some basic tests failed. Check your logic against the PDF examples." << endl;
        }
    }
};

StudentTestRunner runner;

// ==========================================
// PART A: DATA STRUCTURES
// ==========================================

void test_PartA_DataStructures() {
    cout << "\n--- Part A: Data Structures ---" << endl;

    // 1. PlayerTable (Double Hashing)
    // Requirement: Basic Insert and Search
    PlayerTable* table = createPlayerTable();
    runner.runTest("PlayerTable: Insert 'Alice' and Search", [&]() {
        table->insert(101, "Alice");
        return table->search(101) == "Alice";
    }());
    delete table;

    // 2. Leaderboard (Skip List)
    Leaderboard* board = createLeaderboard();

    // Test A: Basic High Score
    runner.runTest("Leaderboard: Add Scores & Get Top 1", [&]() {
        board->addScore(1, 100);
        board->addScore(2, 200); // 2 is Leader
        vector<int> top = board->getTopN(1);
        return (!top.empty() && top[0] == 2);
    }());

    // Test B: Tie-Breaking Visual Example (Crucial!)
    // PDF Visual Example: Player A (ID 10) 500pts, Player B (ID 20) 500pts.
    // Correct Order: ID 10 then ID 20.
    runner.runTest("Leaderboard: Tie-Break (ID 10 before ID 20)", [&]() {
        board->addScore(10, 500);
        board->addScore(20, 500);
        vector<int> top = board->getTopN(2);
        // We expect {10, 20} NOT {20, 10}
        if (top.size() < 2) return false;
        return (top[0] == 10 && top[1] == 20); 
    }());
    
    delete board;

    // 3. AuctionTree (Red-Black Tree)
    // Test 1: Basic insertion without crash
    AuctionTree* tree1 = createAuctionTree();
    runner.runTest("AuctionTree: Basic Insert (no crash)", [&]() {
        tree1->insertItem(1, 100);
        tree1->insertItem(2, 50);
        tree1->insertItem(3, 150);
        return true; // Pass if no crash
    }());
    delete tree1;

    // Test 2: Insert in ascending price order (tests left rotations)
    AuctionTree* tree2 = createAuctionTree();
    runner.runTest("AuctionTree: Ascending Order Insert", [&]() {
        tree2->insertItem(1, 10);
        tree2->insertItem(2, 20);
        tree2->insertItem(3, 30);
        tree2->insertItem(4, 40);
        tree2->insertItem(5, 50);
        return true; // Pass if no crash, tree should balance
    }());
    delete tree2;

    // Test 3: Insert in descending price order (tests right rotations)
    AuctionTree* tree3 = createAuctionTree();
    runner.runTest("AuctionTree: Descending Order Insert", [&]() {
        tree3->insertItem(5, 50);
        tree3->insertItem(4, 40);
        tree3->insertItem(3, 30);
        tree3->insertItem(2, 20);
        tree3->insertItem(1, 10);
        return true; // Pass if no crash, tree should balance
    }());
    delete tree3;

    // Test 4: Mixed insertions (tests various fixup cases)
    AuctionTree* tree4 = createAuctionTree();
    runner.runTest("AuctionTree: Mixed Order Insert", [&]() {
        tree4->insertItem(4, 40);
        tree4->insertItem(2, 20);
        tree4->insertItem(6, 60);
        tree4->insertItem(1, 10);
        tree4->insertItem(3, 30);
        tree4->insertItem(5, 50);
        tree4->insertItem(7, 70);
        return true; // Pass if no crash
    }());
    delete tree4;

    // Test 5: Same price, different IDs (tests secondary sort by ID)
    AuctionTree* tree5 = createAuctionTree();
    runner.runTest("AuctionTree: Same Price Different IDs", [&]() {
        tree5->insertItem(10, 100);
        tree5->insertItem(5, 100);
        tree5->insertItem(15, 100);
        tree5->insertItem(3, 100);
        tree5->insertItem(7, 100);
        return true; // Pass if no crash, sorted by ID when prices equal
    }());
    delete tree5;

    // Test 6: Duplicate ID detection
    AuctionTree* tree6 = createAuctionTree();
    runner.runTest("AuctionTree: Duplicate ID Handling", [&]() {
        tree6->insertItem(1, 100);
        tree6->insertItem(1, 100); // Should be rejected or handled gracefully
        return true; // Pass if no crash
    }());
    delete tree6;

    // Test 7: Large number of insertions
    /*AuctionTree* tree7 = createAuctionTree();
    runner.runTest("AuctionTree: Large Insert (100 items)", [&]() {
        for (int i = 1; i <= 100; i++) {
            tree7->insertItem(i, i * 10);
        }
        return true; // Pass if no crash, tests tree balancing
    }());
    delete tree7;*/

    // Test 8: Zigzag pattern (alternating left-right)
    AuctionTree* tree8 = createAuctionTree();
    runner.runTest("AuctionTree: Zigzag Pattern Insert", [&]() {
        tree8->insertItem(1, 50);
        tree8->insertItem(2, 25);
        tree8->insertItem(3, 75);
        tree8->insertItem(4, 12);
        tree8->insertItem(5, 37);
        tree8->insertItem(6, 62);
        tree8->insertItem(7, 87);
        return true; // Pass if no crash
    }());
    delete tree8;

    // ===== DELETION TESTS =====
    
    // Test 9: Basic deletion - delete leaf node
    AuctionTree* tree9 = createAuctionTree();
    runner.runTest("AuctionTree: Delete Leaf Node", [&]() {
        tree9->insertItem(2, 20);
        tree9->insertItem(1, 10);
        tree9->insertItem(3, 30);
        tree9->deleteItem(1); // Delete leaf
        return true; // Pass if no crash
    }());
    delete tree9;

    // Test 10: Delete node with one child
    AuctionTree* tree10 = createAuctionTree();
    runner.runTest("AuctionTree: Delete Node with One Child", [&]() {
        tree10->insertItem(2, 20);
        tree10->insertItem(1, 10);
        tree10->insertItem(4, 40);
        tree10->insertItem(3, 30);
        tree10->deleteItem(4); // Node 4 has only left child
        return true; // Pass if no crash
    }());
    delete tree10;

    // Test 11: Delete node with two children
    AuctionTree* tree11 = createAuctionTree();
    runner.runTest("AuctionTree: Delete Node with Two Children", [&]() {
        tree11->insertItem(4, 40);
        tree11->insertItem(2, 20);
        tree11->insertItem(6, 60);
        tree11->insertItem(1, 10);
        tree11->insertItem(3, 30);
        tree11->insertItem(5, 50);
        tree11->insertItem(7, 70);
        tree11->deleteItem(4); // Delete node with two children
        return true; // Pass if no crash
    }());
    delete tree11;

    // Test 12: Delete root node
    AuctionTree* tree12 = createAuctionTree();
    runner.runTest("AuctionTree: Delete Root Node", [&]() {
        tree12->insertItem(2, 20);
        tree12->insertItem(1, 10);
        tree12->insertItem(3, 30);
        tree12->deleteItem(2); // Delete root
        return true; // Pass if no crash
    }());
    delete tree12;

    // Test 13: Delete non-existent item
    AuctionTree* tree13 = createAuctionTree();
    runner.runTest("AuctionTree: Delete Non-Existent Item", [&]() {
        tree13->insertItem(1, 10);
        tree13->insertItem(2, 20);
        tree13->deleteItem(999); // Should handle gracefully
        return true; // Pass if no crash
    }());
    delete tree13;

    // Test 14: Delete from single-node tree
    AuctionTree* tree14 = createAuctionTree();
    runner.runTest("AuctionTree: Delete Only Node", [&]() {
        tree14->insertItem(1, 10);
        tree14->deleteItem(1); // Tree becomes empty
        return true; // Pass if no crash
    }());
    delete tree14;

    // Test 15: Multiple deletions in sequence
    AuctionTree* tree15 = createAuctionTree();
    runner.runTest("AuctionTree: Multiple Sequential Deletions", [&]() {
        tree15->insertItem(4, 40);
        tree15->insertItem(2, 20);
        tree15->insertItem(6, 60);
        tree15->insertItem(1, 10);
        tree15->insertItem(3, 30);
        tree15->insertItem(5, 50);
        tree15->insertItem(7, 70);
        tree15->deleteItem(1);
        tree15->deleteItem(7);
        tree15->deleteItem(4);
        tree15->deleteItem(2);
        return true; // Pass if no crash, tests rebalancing
    }());
    delete tree15;

    // Test 16: Delete all nodes one by one
    AuctionTree* tree16 = createAuctionTree();
    runner.runTest("AuctionTree: Delete All Nodes", [&]() {
        tree16->insertItem(1, 10);
        tree16->insertItem(2, 20);
        tree16->insertItem(3, 30);
        tree16->insertItem(4, 40);
        tree16->insertItem(5, 50);
        tree16->deleteItem(3);
        tree16->deleteItem(1);
        tree16->deleteItem(5);
        tree16->deleteItem(2);
        tree16->deleteItem(4);
        return true; // Pass if no crash
    }());
    delete tree16;

    // Test 17: Delete with same prices (tests ID-based ordering)
    AuctionTree* tree17 = createAuctionTree();
    runner.runTest("AuctionTree: Delete Same Price Different IDs", [&]() {
        tree17->insertItem(10, 100);
        tree17->insertItem(5, 100);
        tree17->insertItem(15, 100);
        tree17->insertItem(3, 100);
        tree17->insertItem(7, 100);
        tree17->deleteItem(10); // Delete middle ID
        tree17->deleteItem(3);  // Delete smallest ID
        tree17->deleteItem(15); // Delete largest ID
        return true; // Pass if no crash
    }());
    delete tree17;

    // Test 18: Interleaved insert and delete
    AuctionTree* tree18 = createAuctionTree();
    runner.runTest("AuctionTree: Interleaved Insert/Delete", [&]() {
        tree18->insertItem(1, 10);
        tree18->insertItem(2, 20);
        tree18->deleteItem(1);
        tree18->insertItem(3, 30);
        tree18->deleteItem(2);
        tree18->insertItem(4, 40);
        tree18->deleteItem(3);
        tree18->insertItem(5, 50);
        return true; // Pass if no crash
    }());
    delete tree18;

    // Test 19: Delete causing black height fixup (complex RB case)
    AuctionTree* tree19 = createAuctionTree();
    runner.runTest("AuctionTree: Delete Triggering Fixup Cases", [&]() {
        // Build a more complex tree to trigger various fixup cases
        for (int i = 1; i <= 15; i++) {
            tree19->insertItem(i, i * 10);
        }
        tree19->deleteItem(1);
        tree19->deleteItem(2);
        tree19->deleteItem(3);
        tree19->deleteItem(8);
        tree19->deleteItem(12);
        return true; // Pass if no crash, tests complex rebalancing
    }());
    delete tree19;

}

// ==========================================
// PART B: INVENTORY SYSTEM
// ==========================================

void test_PartB_Inventory() {
    cout << "\n--- Part B: Inventory System ---" << endl;

    // 1. Loot Splitting (Partition)
    // PDF Example: coins = {1, 2, 4} -> Best split {4} vs {1,2} -> Diff 1
    runner.runTest("LootSplit: {1, 2, 4} -> Diff 1", [&]() {
        vector<int> coins = {1, 2, 4};
        return InventorySystem::optimizeLootSplit(3, coins) == 1;
    }());

    // 2. Inventory Packer (Knapsack)
    // PDF Example: Cap=10, Items={{1,10}, {2,20}, {3,30}}. All fit. Value=60.
    runner.runTest("Knapsack: Cap 10, All Fit -> Value 60", [&]() {
        vector<pair<int, int>> items = {{1, 10}, {2, 20}, {3, 30}};
        return InventorySystem::maximizeCarryValue(10, items) == 60;
    }());

    // 3. Chat Autocorrect (String DP)
    // PDF Example: "uu" -> "uu" or "w" -> 2 possibilities
    runner.runTest("ChatDecorder: 'uu' -> 2 Possibilities", [&]() {
        return InventorySystem::countStringPossibilities("uu") == 2;
    }());
}

// ==========================================
// PART C: WORLD NAVIGATOR
// ==========================================

void test_PartC_Navigator() {
    cout << "\n--- Part C: World Navigator ---" << endl;

    // 1. Safe Passage (Path Exists)
    // PDF Example: 0-1, 1-2. Path 0->2 exists.
    runner.runTest("PathExists: 0->1->2 -> True", [&]() {
        vector<vector<int>> edges = {{0, 1}, {1, 2}};
        return WorldNavigator::pathExists(3, edges, 0, 2) == true;
    }());

    // 2. The Bribe (MST)
    // PDF Example: 3 Nodes. Roads: {0,1,10}, {1,2,5}, {0,2,20}. Rate=1.
    // MST should pick 10 and 5. Total 15.
    runner.runTest("MinBribeCost: Triangle Graph -> Cost 15", [&]() {
        vector<vector<int>> roads = {
            {0, 1, 10, 0}, 
            {1, 2, 5, 0}, 
            {0, 2, 20, 0}
        };
        // n=3, m=3, goldRate=1, silverRate=1
        return WorldNavigator::minBribeCost(3, 3, 1, 1, roads) == 15;
    }());

    // 3. Teleporter (Binary Sum APSP)
    // PDF Example: 0-1 (1), 1-2 (2). Distances: 1, 2, 3. Sum=6 -> "110"
    runner.runTest("BinarySum: Line Graph -> '110'", [&]() {
        vector<vector<int>> roads = {
            {0, 1, 1},
            {1, 2, 2}
        };
        return WorldNavigator::sumMinDistancesBinary(3, roads) == "110";
    }());
}

// ==========================================
// PART D: SERVER KERNEL
// ==========================================

void test_PartD_Kernel() {
    cout << "\n--- Part D: Server Kernel ---" << endl;

    // 1. Task Scheduler
    // PDF Example: Tasks={A, A, B}, n=2.
    // Order: A -> B -> idle -> A. Total intervals: 4.
    runner.runTest("Scheduler: {A, A, B}, n=2 -> 4 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'B'};
        return ServerKernel::minIntervals(tasks, 2) == 4;
    }());
}

int main() {
    cout << "Arcadia Engine - Student Happy Path Tests" << endl;
    cout << "-----------------------------------------" << endl;

    test_PartA_DataStructures();
    test_PartB_Inventory();
    test_PartC_Navigator();
    test_PartD_Kernel();

    runner.printSummary();

    return 0;
}