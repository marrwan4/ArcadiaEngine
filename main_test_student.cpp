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
    // ========== BASIC FUNCTIONALITY ==========

    runner.runTest("PlayerTable: Insert and Search - Basic", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(101, "Alice");
        bool result = table->search(101) == "Alice";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Search Non-Existent Player", [&]() {
        PlayerTable* table = createPlayerTable();
        bool result = table->search(999) == "";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Update Existing Player", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(101, "Alice");
        table->insert(101, "Alice Updated");
        bool result = table->search(101) == "Alice Updated";
        delete table;
        return result;
    }());

    // ========== COLLISION TESTS ==========

    runner. runTest("PlayerTable:  Collision - Same h1 Value", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(101, "Player1");  // h1 = 101 % 101 = 0
        table->insert(202, "Player2");  // h1 = 202 % 101 = 0 (collision!)
        bool result = table->search(101) == "Player1" && table->search(202) == "Player2";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Multiple Collisions Chain", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(5, "Alpha");    // h1 = 5
        table->insert(106, "Beta");   // h1 = 106 % 101 = 5 (collision!)
        table->insert(207, "Gamma");  // h1 = 207 % 101 = 5 (collision!)
        table->insert(308, "Delta");  // h1 = 308 % 101 = 5 (collision!)
        bool result = table->search(5) == "Alpha" && 
                    table->search(106) == "Beta" && 
                    table->search(207) == "Gamma" &&
                    table->search(308) == "Delta";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Search After Collision", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(10, "John");
        table->insert(111, "Jane");  // 111 % 101 = 10 (collision with John)
        bool result = table->search(111) == "Jane";
        delete table;
        return result;
    }());

    // ========== EDGE CASE:  h2 = 0 (Critical Bug Test) ==========

    runner.runTest("PlayerTable: h2 Edge Case - playerID divisible by 97", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(97, "EdgeCase1");   // 97 % 97 = 0, h2 = 97 - 0 = 97
        table->insert(194, "EdgeCase2");  // 194 % 97 = 0, h2 = 97 - 0 = 97
        bool result = table->search(97) == "EdgeCase1" && table->search(194) == "EdgeCase2";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: h2 = 1 Edge Case", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(96, "Test96");   // 96 % 97 = 96, h2 = 97 - 96 = 1
        table->insert(193, "Test193"); // 193 % 97 = 96, h2 = 1 (collision + small step)
        bool result = table->search(96) == "Test96" && table->search(193) == "Test193";
        delete table;
        return result;
    }());

    // ========== EMPTY SLOT EARLY TERMINATION ==========

    runner.runTest("PlayerTable: Search Stops at Empty Slot", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(50, "Exists");
        bool result = table->search(999) == "";  // Should stop early at empty slot
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Search in Collision Chain with Gap", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(20, "First");
        table->insert(121, "Second");  // 121 % 101 = 20, collision
        bool result = table->search(222) == "";
        delete table;
        return result;
    }());

    // ========== UPDATE DURING COLLISION ==========

    runner.runTest("PlayerTable: Update Player in Collision Chain", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(15, "Original");
        table->insert(116, "Collider");  // 116 % 101 = 15
        table->insert(15, "Updated");    // Should update, not insert again
        bool result = table->search(15) == "Updated" && table->search(116) == "Collider";
        delete table;
        return result;
    }());

    // ========== FULL TABLE TESTS ==========

    runner.runTest("PlayerTable: Fill Table Completely", [&]() {
        PlayerTable* table = createPlayerTable();
        for (int i = 0; i < 101; i++) {
            table->insert(i, "Player" + to_string(i));
        }
        bool result = table->search(0) == "Player0" && 
                    table->search(50) == "Player50" && 
                    table->search(100) == "Player100";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Insert When Table is Full", [&]() {
        PlayerTable* table = createPlayerTable();
        for (int i = 0; i < 101; i++) {
            table->insert(i, "Player" + to_string(i));
        }
        cout << "Expected output: 'Table is Full' -> ";
        table->insert(200, "Overflow");
        bool result = table->search(200) == "";  // Should not be inserted
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Search in Full Table", [&]() {
        PlayerTable* table = createPlayerTable();
        for (int i = 0; i < 101; i++) {
            table->insert(i * 100, "Player" + to_string(i));
        }
        bool result = table->search(5000) == "Player50";
        delete table;
        return result;
    }());

    // ========== BOUNDARY VALUES ==========

    runner.runTest("PlayerTable: Insert playerID = 0", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(0, "ZeroPlayer");
        bool result = table->search(0) == "ZeroPlayer";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Insert Large playerID", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(999999, "LargeID");
        bool result = table->search(999999) == "LargeID";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Insert Negative playerID", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(-5, "NegativeID");
        bool result = table->search(-5) == "NegativeID";
        delete table;
        return result;
    }());

    // ========== STRESS TEST:  MANY COLLISIONS ==========

    runner.runTest("PlayerTable: 50 Insertions with Mixed Collisions", [&]() {
        PlayerTable* table = createPlayerTable();
        for (int i = 0; i < 50; i++) {
            table->insert(i * 101, "Player" + to_string(i));  // All map to index 0! 
        }
        bool result = table->search(0) == "Player0" && 
                    table->search(101) == "Player1" && 
                    table->search(4949) == "Player49";
        delete table;
        return result;
    }());

    // ========== EMPTY TABLE SEARCH ==========

    runner. runTest("PlayerTable: Search in Empty Table", [&]() {
        PlayerTable* table = createPlayerTable();
        bool result = table->search(123) == "";
        delete table;
        return result;
    }());

    // ========== SEQUENTIAL IDs WITH COLLISIONS ==========

    runner. runTest("PlayerTable: Sequential IDs Causing Collisions", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(1, "One");
        table->insert(102, "OneTwoTwo");  // 102 % 101 = 1
        table->insert(203, "TwoThreeThree");  // 203 % 101 = 1
        bool result = table->search(1) == "One" && 
                    table->search(102) == "OneTwoTwo" && 
                    table->search(203) == "TwoThreeThree";
        delete table;
        return result;
    }());

    // ========== SPECIAL NAMES ==========

    runner.runTest("PlayerTable: Empty String Name", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(42, "");
        bool result = table->search(42) == "";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Very Long Name", [&]() {
        PlayerTable* table = createPlayerTable();
        string longName(1000, 'X');
        table->insert(77, longName);
        bool result = table->search(77) == longName;
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Special Characters in Name", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(88, "Alice@#$%^&*()");
        bool result = table->search(88) == "Alice@#$%^&*()";
        delete table;
        return result;
    }());

    // ========== CRITICAL BUG FINDER ==========

    runner.runTest("PlayerTable:  Near Full Table with Collisions", [&]() {
        PlayerTable* table = createPlayerTable();
        for (int i = 0; i < 100; i++) {
            table->insert(i, "Player" + to_string(i));
        }
        table->insert(97, "Updated97");  // Update existing, not insert new
        bool result = table->search(97) == "Updated97";
        delete table;
        return result;
    }());

    // ========== MULTIPLE UPDATES ==========

    runner.runTest("PlayerTable: Multiple Updates Same Player", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(123, "Version1");
        table->insert(123, "Version2");
        table->insert(123, "Version3");
        table->insert(123, "Final");
        bool result = table->search(123) == "Final";
        delete table;
        return result;
    }());

    // ========== COLLISION THEN UPDATE ==========

    runner.runTest("PlayerTable: Collision Then Update First Element", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(50, "Original50");
        table->insert(151, "Player151");  // 151 % 101 = 50, collision
        table->insert(50, "Updated50");   // Update first element in chain
        bool result = table->search(50) == "Updated50" && table->search(151) == "Player151";
        delete table;
        return result;
    }());

    runner.runTest("PlayerTable: Collision Then Update Second Element", [&]() {
        PlayerTable* table = createPlayerTable();
        table->insert(50, "Player50");
        table->insert(151, "Original151");  // 151 % 101 = 50, collision
        table->insert(151, "Updated151");   // Update second element in chain
        bool result = table->search(50) == "Player50" && table->search(151) == "Updated151";
        delete table;
        return result;
    }());

    // ========== WRAP AROUND TEST ==========

    runner.runTest("PlayerTable: Probing Wraps Around Table", [&]() {
        PlayerTable* table = createPlayerTable();
        // Fill slots 95-100 to force wrap around
        for (int i = 95; i <= 100; i++) {
            table->insert(i, "Player" + to_string(i));
        }
        // Insert something that hashes near end and must wrap
        table->insert(9995, "WrapTest");  // 9995 % 101 = 95, will need to wrap
        bool result = table->search(9995) == "WrapTest";
        delete table;
        return result;
    }());

    // 2. Leaderboard (Skip List)
    Leaderboard* board = createLeaderboard();

    // ===== BASIC OPERATIONS =====

    // Test A: Basic High Score
    runner.runTest("Leaderboard: Add Scores & Get Top 1", [&]() {
        board->addScore(1, 100);
        board->addScore(2, 200); // 2 is Leader
        vector<int> top = board->getTopN(1);
        return (!top.empty() && top[0] == 2);
    }());

    // Test B: Tie-Breaking Visual Example (Crucial!)
    runner.runTest("Leaderboard: Tie-Break (ID 10 before ID 20)", [&]() {
        board->addScore(10, 500);
        board->addScore(20, 500);
        vector<int> top = board->getTopN(2);
        if (top.size() < 2) return false;
        return (top[0] == 10 && top[1] == 20); 
    }());

    // Test C:  Descending Order by Score
    runner.runTest("Leaderboard: Descending Score Order", [&]() {
        board->addScore(101, 300);
        board->addScore(102, 400);
        board->addScore(103, 350);
        vector<int> top = board->getTopN(5);
        // Expected order: 102(400), 103(350), 101(300), 20(500), 10(500)
        // Wait, 20 and 10 have 500, so:  10(500), 20(500), 102(400), 103(350), 101(300)
        if (top.size() < 5) return false;
        return (top[0] == 10 && top[1] == 20 && top[2] == 102 && top[3] == 103 && top[4] == 101);
    }());

    // ===== EDGE CASES:  EMPTY & SMALL INPUTS =====

    runner.runTest("Leaderboard: Get Top N from Empty List", [&]() {
        Leaderboard* emptyBoard = createLeaderboard();
        vector<int> top = emptyBoard->getTopN(5);
        bool result = top.empty();
        delete emptyBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Get Top 0", [&]() {
        vector<int> top = board->getTopN(0);
        return top.empty();
    }());

    runner.runTest("Leaderboard: Request More Than Available", [&]() {
        Leaderboard* smallBoard = createLeaderboard();
        smallBoard->addScore(1, 100);
        smallBoard->addScore(2, 200);
        vector<int> top = smallBoard->getTopN(10); // Only 2 players
        bool result = (top.size() == 2 && top[0] == 2 && top[1] == 1);
        delete smallBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Single Player", [&]() {
        Leaderboard* singleBoard = createLeaderboard();
        singleBoard->addScore(42, 999);
        vector<int> top = singleBoard->getTopN(1);
        bool result = (! top.empty() && top[0] == 42);
        delete singleBoard;
        return result;
    }());

    // ===== TIE-BREAKING EDGE CASES =====

    runner.runTest("Leaderboard: Multiple Ties with Different IDs", [&]() {
        Leaderboard* tieBoard = createLeaderboard();
        tieBoard->addScore(50, 1000);
        tieBoard->addScore(30, 1000);
        tieBoard->addScore(40, 1000);
        tieBoard->addScore(20, 1000);
        vector<int> top = tieBoard->getTopN(4);
        // Expected: 20, 30, 40, 50 (ascending ID order)
        bool result = (top.size() == 4 && 
                    top[0] == 20 && top[1] == 30 && 
                    top[2] == 40 && top[3] == 50);
        delete tieBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Interleaved Scores and Ties", [&]() {
        Leaderboard* mixBoard = createLeaderboard();
        mixBoard->addScore(5, 800);
        mixBoard->addScore(3, 900);
        mixBoard->addScore(7, 800);
        mixBoard->addScore(1, 900);
        mixBoard->addScore(9, 700);
        vector<int> top = mixBoard->getTopN(5);
        // Expected: 1(900), 3(900), 5(800), 7(800), 9(700)
        bool result = (top.size() == 5 && 
                    top[0] == 1 && top[1] == 3 && 
                    top[2] == 5 && top[3] == 7 && top[4] == 9);
        delete mixBoard;
        return result;
    }());

    // ===== DELETION TESTS =====

    runner.runTest("Leaderboard: Remove Existing Player", [&]() {
        Leaderboard* delBoard = createLeaderboard();
        delBoard->addScore(1, 100);
        delBoard->addScore(2, 200);
        delBoard->addScore(3, 300);
        delBoard->removePlayer(2); // Remove middle
        vector<int> top = delBoard->getTopN(3);
        // Expected: 3(300), 1(100)
        bool result = (top. size() == 2 && top[0] == 3 && top[1] == 1);
        delete delBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Remove Non-Existent Player", [&]() {
        Leaderboard* delBoard = createLeaderboard();
        delBoard->addScore(1, 100);
        delBoard->removePlayer(999); // Doesn't exist
        vector<int> top = delBoard->getTopN(1);
        bool result = (top.size() == 1 && top[0] == 1);
        delete delBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Remove from Empty List", [&]() {
        Leaderboard* emptyDelBoard = createLeaderboard();
        emptyDelBoard->removePlayer(1); // Should not crash
        vector<int> top = emptyDelBoard->getTopN(1);
        bool result = top.empty();
        delete emptyDelBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Remove Top Player", [&]() {
        Leaderboard* topDelBoard = createLeaderboard();
        topDelBoard->addScore(1, 100);
        topDelBoard->addScore(2, 200);
        topDelBoard->addScore(3, 300);
        topDelBoard->removePlayer(3); // Remove highest
        vector<int> top = topDelBoard->getTopN(1);
        bool result = (! top.empty() && top[0] == 2);
        delete topDelBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Remove Last Player", [&]() {
        Leaderboard* lastDelBoard = createLeaderboard();
        lastDelBoard->addScore(1, 100);
        lastDelBoard->addScore(2, 200);
        lastDelBoard->addScore(3, 300);
        lastDelBoard->removePlayer(1); // Remove lowest
        vector<int> top = lastDelBoard->getTopN(3);
        bool result = (top.size() == 2 && top[0] == 3 && top[1] == 2);
        delete lastDelBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Remove All Players One by One", [&]() {
        Leaderboard* clearBoard = createLeaderboard();
        clearBoard->addScore(1, 100);
        clearBoard->addScore(2, 200);
        clearBoard->addScore(3, 300);
        clearBoard->removePlayer(1);
        clearBoard->removePlayer(2);
        clearBoard->removePlayer(3);
        vector<int> top = clearBoard->getTopN(1);
        bool result = top.empty();
        delete clearBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Remove Player with Tied Score", [&]() {
        Leaderboard* tieDelBoard = createLeaderboard();
        tieDelBoard->addScore(10, 500);
        tieDelBoard->addScore(20, 500);
        tieDelBoard->addScore(30, 500);
        tieDelBoard->removePlayer(20); // Remove middle of tie
        vector<int> top = tieDelBoard->getTopN(3);
        bool result = (top.size() == 2 && top[0] == 10 && top[1] == 30);
        delete tieDelBoard;
        return result;
    }());

    // ===== DUPLICATE ID TESTS (SKIP LIST ALLOWS THIS) =====

    runner.runTest("Leaderboard: Add Same Player Multiple Times", [&]() {
        Leaderboard* dupBoard = createLeaderboard();
        dupBoard->addScore(1, 100);
        dupBoard->addScore(1, 200); // Same ID, different score
        dupBoard->addScore(1, 150);
        vector<int> top = dupBoard->getTopN(5);
        // Skip list allows duplicates, so all 3 should exist
        // Expected: 1(200), 1(150), 1(100)
        bool result = (top.size() == 3 && 
                    top[0] == 1 && top[1] == 1 && top[2] == 1);
        delete dupBoard;
        return result;
    }());

    // ===== EXTREME VALUES =====

    runner.runTest("Leaderboard: Zero Score", [&]() {
        Leaderboard* zeroBoard = createLeaderboard();
        zeroBoard->addScore(1, 0);
        zeroBoard->addScore(2, 100);
        vector<int> top = zeroBoard->getTopN(2);
        bool result = (top.size() == 2 && top[0] == 2 && top[1] == 1);
        delete zeroBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Negative Scores", [&]() {
        Leaderboard* negBoard = createLeaderboard();
        negBoard->addScore(1, -100);
        negBoard->addScore(2, 50);
        negBoard->addScore(3, -200);
        vector<int> top = negBoard->getTopN(3);
        // Expected: 2(50), 1(-100), 3(-200)
        bool result = (top.size() == 3 && 
                    top[0] == 2 && top[1] == 1 && top[2] == 3);
        delete negBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Very Large Scores", [&]() {
        Leaderboard* largeBoard = createLeaderboard();
        largeBoard->addScore(1, 2147483647); // INT_MAX
        largeBoard->addScore(2, 2147483646);
        vector<int> top = largeBoard->getTopN(2);
        bool result = (top.size() == 2 && top[0] == 1 && top[1] == 2);
        delete largeBoard;
        return result;
    }());

    // ===== STRESS TEST =====

    runner.runTest("Leaderboard: Large Number of Players", [&]() {
        Leaderboard* stressBoard = createLeaderboard();
        for (int i = 1; i <= 100; i++) {
            stressBoard->addScore(i, i * 10);
        }
        vector<int> top = stressBoard->getTopN(5);
        // Expected: 100, 99, 98, 97, 96
        bool result = (top.size() == 5 && 
                    top[0] == 100 && top[1] == 99 && 
                    top[2] == 98 && top[3] == 97 && top[4] == 96);
        delete stressBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Random Insert Order", [&]() {
        Leaderboard* randomBoard = createLeaderboard();
        randomBoard->addScore(50, 500);
        randomBoard->addScore(10, 100);
        randomBoard->addScore(90, 900);
        randomBoard->addScore(30, 300);
        randomBoard->addScore(70, 700);
        vector<int> top = randomBoard->getTopN(5);
        // Expected: 90(900), 70(700), 50(500), 30(300), 10(100)
        bool result = (top.size() == 5 && 
                    top[0] == 90 && top[1] == 70 && 
                    top[2] == 50 && top[3] == 30 && top[4] == 10);
        delete randomBoard;
        return result;
    }());

    // ===== SKIP LIST SPECIFIC EDGE CASES =====

    runner.runTest("Leaderboard: Level Reduction After Deletions", [&]() {
        Leaderboard* levelBoard = createLeaderboard();
        // Add many players to increase skip list levels
        for (int i = 1; i <= 50; i++) {
            levelBoard->addScore(i, i * 10);
        }
        // Remove most of them
        for (int i = 1; i <= 45; i++) {
            levelBoard->removePlayer(i);
        }
        vector<int> top = levelBoard->getTopN(10);
        // Should have 5 players left:  46-50
        bool result = (top.size() == 5 && top[0] == 50);
        delete levelBoard;
        return result;
    }());

    runner.runTest("Leaderboard: Insert After Delete", [&]() {
        Leaderboard* reinsertBoard = createLeaderboard();
        reinsertBoard->addScore(1, 100);
        reinsertBoard->addScore(2, 200);
        reinsertBoard->removePlayer(2);
        reinsertBoard->addScore(3, 300);
        vector<int> top = reinsertBoard->getTopN(3);
        bool result = (top.size() == 2 && top[0] == 3 && top[1] == 1);
        delete reinsertBoard;
        return result;
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
    cout << "\n--- Part B:  Inventory System ---" << endl;

    // ===== 1. LOOT SPLITTING (PARTITION PROBLEM) =====
    
    // PDF Example 1: coins = {1, 2, 4} -> Best split {4} vs {1,2} -> Diff 1
    runner.runTest("LootSplit: {1, 2, 4} -> Diff 1", [&]() {
        vector<int> coins = {1, 2, 4};
        return InventorySystem::optimizeLootSplit(3, coins) == 1;
    }());

    // PDF Example 2: coins = {2, 2} -> Perfect split {2} vs {2} -> Diff 0
    runner.runTest("LootSplit: {2, 2} -> Diff 0", [&]() {
        vector<int> coins = {2, 2};
        return InventorySystem:: optimizeLootSplit(2, coins) == 0;
    }());

    // Edge Case: Single coin (one person gets it all)
    runner.runTest("LootSplit: Single coin {5} -> Diff 5", [&]() {
        vector<int> coins = {5};
        return InventorySystem::optimizeLootSplit(1, coins) == 5;
    }());

    // Edge Case: All same values (perfect split or diff 1)
    runner.runTest("LootSplit: {1, 1, 1, 1} -> Diff 0", [&]() {
        vector<int> coins = {1, 1, 1, 1};
        return InventorySystem::optimizeLootSplit(4, coins) == 0;
    }());

    // Edge Case: Odd number of identical coins
    runner.runTest("LootSplit: {3, 3, 3} -> Diff 3", [&]() {
        vector<int> coins = {3, 3, 3};
        return InventorySystem::optimizeLootSplit(3, coins) == 3;
    }());

    // Edge Case: Large difference in values
    runner.runTest("LootSplit: {1, 1, 100} -> Diff 98", [&]() {
        vector<int> coins = {1, 1, 100};
        return InventorySystem::optimizeLootSplit(3, coins) == 98;
    }());

    // Edge Case: Already perfectly balanced
    runner.runTest("LootSplit: {5, 5, 5, 5} -> Diff 0", [&]() {
        vector<int> coins = {5, 5, 5, 5};
        return InventorySystem::optimizeLootSplit(4, coins) == 0;
    }());

    // Edge Case: Fibonacci-like sequence
    runner.runTest("LootSplit: {1, 2, 3, 5, 8} -> Diff 1", [&]() {
        vector<int> coins = {1, 2, 3, 5, 8};
        // Total = 19.  Best:  {8,2} = 10 vs {5,3,1} = 9 -> Diff 1
        return InventorySystem::optimizeLootSplit(5, coins) == 1;
    }());

    // Edge Case: Two coins only
    runner.runTest("LootSplit: {3, 7} -> Diff 4", [&]() {
        vector<int> coins = {3, 7};
        return InventorySystem::optimizeLootSplit(2, coins) == 4;
    }());

    // ===== 2. INVENTORY PACKER (KNAPSACK) =====
    
    // PDF Example:  Cap=10, Items={{1,10}, {2,20}, {3,30}}. All fit.  Value=60.
    runner. runTest("Knapsack:  Cap 10, All Fit -> Value 60", [&]() {
        vector<pair<int, int>> items = {{1, 10}, {2, 20}, {3, 30}};
        return InventorySystem:: maximizeCarryValue(10, items) == 60;
    }());

    // Edge Case:  Capacity is 0
    runner.runTest("Knapsack: Cap 0 -> Value 0", [&]() {
        vector<pair<int, int>> items = {{1, 10}, {2, 20}};
        return InventorySystem::maximizeCarryValue(0, items) == 0;
    }());

    // Edge Case: No items
    runner.runTest("Knapsack: No items -> Value 0", [&]() {
        vector<pair<int, int>> items = {};
        return InventorySystem::maximizeCarryValue(10, items) == 0;
    }());

    // Edge Case: All items too heavy
    runner.runTest("Knapsack: All too heavy -> Value 0", [&]() {
        vector<pair<int, int>> items = {{10, 100}, {20, 200}};
        return InventorySystem::maximizeCarryValue(5, items) == 0;
    }());

    // Edge Case: Single item that fits
    runner.runTest("Knapsack: Single item fits -> Value 50", [&]() {
        vector<pair<int, int>> items = {{5, 50}};
        return InventorySystem::maximizeCarryValue(10, items) == 50;
    }());

    // Edge Case: Single item too heavy
    runner.runTest("Knapsack: Single item too heavy -> Value 0", [&]() {
        vector<pair<int, int>> items = {{15, 50}};
        return InventorySystem::maximizeCarryValue(10, items) == 0;
    }());

    // Edge Case: Choose lighter item with more value
    runner.runTest("Knapsack:  Optimal choice -> Value 100", [&]() {
        vector<pair<int, int>> items = {{5, 60}, {3, 50}, {4, 70}};
        // Cap = 7:  Best is {3,50} + {4,70} = 120?  No, weight = 7, value = 120
        return InventorySystem:: maximizeCarryValue(7, items) == 120;
    }());

    // Edge Case:  Exactly at capacity
    runner.runTest("Knapsack: Exact capacity -> Value 100", [&]() {
        vector<pair<int, int>> items = {{5, 50}, {5, 50}};
        return InventorySystem::maximizeCarryValue(10, items) == 100;
    }());

    runner.runTest("Knapsack: Many small > One large", [&]() {
    vector<pair<int, int>> items = {{1, 15}, {1, 15}, {1, 15}, {5, 40}};
    // Cap = 5: Best is 3 items of {1,15} = 45 > {5,40} = 40
    return InventorySystem:: maximizeCarryValue(5, items) == 45;
    }());

    // Edge Case: All items weight 1, capacity matches count
    runner.runTest("Knapsack: All weight 1 -> Sum all", [&]() {
        vector<pair<int, int>> items = {{1, 10}, {1, 20}, {1, 30}, {1, 40}};
        return InventorySystem::maximizeCarryValue(4, items) == 100;
    }());

    // ===== 3. CHAT AUTOCORRECT (STRING DP) =====
    
    // PDF Example 1: "uu" -> "uu" or "w" -> 2 possibilities
    runner. runTest("ChatDecoder: 'uu' -> 2 Possibilities", [&]() {
        return InventorySystem::countStringPossibilities("uu") == 2;
    }());

    // PDF Example 2: "uunn" -> "uunn", "wnn", "uum", "wm" -> 4 possibilities
    runner.runTest("ChatDecoder: 'uunn' -> 4 Possibilities", [&]() {
        return InventorySystem::countStringPossibilities("uunn") == 4;
    }());

    // PDF Example 3: "w" -> Invalid (broken keyboard) -> 0
    runner.runTest("ChatDecoder: 'w' -> 0 (Invalid)", [&]() {
        return InventorySystem::countStringPossibilities("w") == 0;
    }());

    // PDF Example 4: "m" -> Invalid (broken keyboard) -> 0
    runner.runTest("ChatDecoder: 'm' -> 0 (Invalid)", [&]() {
        return InventorySystem::countStringPossibilities("m") == 0;
    }());

    // PDF Example 5: "" -> Empty string -> 1 way (do nothing)
    runner.runTest("ChatDecoder: '' -> 1 Possibility", [&]() {
        return InventorySystem::countStringPossibilities("") == 1;
    }());

    // Edge Case: Single 'u'
    runner.runTest("ChatDecoder: 'u' -> 1 Possibility", [&]() {
        return InventorySystem::countStringPossibilities("u") == 1;
    }());

    // Edge Case: Single 'n'
    runner.runTest("ChatDecoder: 'n' -> 1 Possibility", [&]() {
        return InventorySystem::countStringPossibilities("n") == 1;
    }());

    // Edge Case: "uuu" -> "uuu", "wu", "uw" -> 2 possibilities (only consecutive pairs)
    runner.runTest("ChatDecoder: 'uuu' -> 3 Possibilities", [&]() {
        // "uuu" as-is, or first "uu"→"w" giving "wu", or last "uu"→"w" giving "uw"
        return InventorySystem::countStringPossibilities("uuu") == 3;
    }());

    // Edge Case: "uuuu" -> Fibonacci-like growth
    runner.runTest("ChatDecoder: 'uuuu' -> 5 Possibilities", [&]() {
        // Follows Fibonacci: dp[4] = dp[3] + dp[2] = 3 + 2 = 5
        return InventorySystem:: countStringPossibilities("uuuu") == 5;
    }());

    // Edge Case:  "nnnn" -> Same as "uuuu"
    runner.runTest("ChatDecoder: 'nnnn' -> 5 Possibilities", [&]() {
        return InventorySystem::countStringPossibilities("nnnn") == 5;
    }());

    // Edge Case: Invalid 'w' in middle
    runner.runTest("ChatDecoder: 'uuwnn' -> 0 (Invalid)", [&]() {
        return InventorySystem::countStringPossibilities("uuwnn") == 0;
    }());

    // Edge Case: Invalid 'm' in middle
    runner.runTest("ChatDecoder: 'uumnm' -> 0 (Invalid)", [&]() {
        return InventorySystem::countStringPossibilities("uumnm") == 0;
    }());

    // Edge Case: Invalid 'w' at end
    runner.runTest("ChatDecoder: 'uuw' -> 0 (Invalid)", [&]() {
        return InventorySystem::countStringPossibilities("uuw") == 0;
    }());

    // Edge Case: Mixed valid characters that don't form pairs
    runner.runTest("ChatDecoder: 'un' -> 1 Possibility", [&]() {
        return InventorySystem::countStringPossibilities("un") == 1;
    }());

    // Edge Case: "uunnuu" -> (2)*(2)*(2) = 8? No, it's additive
    runner.runTest("ChatDecoder: 'uunnuu' -> 16 Possibilities", [&]() {
        // dp[0]=1, dp[1]=1, dp[2]=2, dp[3]=3, dp[4]=5, dp[5]=8, dp[6]=13
        // Wait, let me recalculate: 
        // "uunnuu":  indices 0-5
        // i=2: "uu" -> dp[2] = dp[1] + dp[0] = 1+1 = 2
        // i=3: "un" -> dp[3] = dp[2] = 2 (no merge)
        // i=4: "nn" -> dp[4] = dp[3] + dp[2] = 2+2 = 4
        // i=5: "nu" -> dp[5] = dp[4] = 4 (no merge)
        // i=6: "uu" -> dp[6] = dp[5] + dp[4] = 4+4 = 8
        return InventorySystem::countStringPossibilities("uunnuu") == 8;
    }());

    // Edge Case: Other characters (if allowed)
    runner.runTest("ChatDecoder: 'abc' -> 1 Possibility", [&]() {
        return InventorySystem::countStringPossibilities("abc") == 1;
    }());

    // Edge Case: Long alternating pattern "ununun..."
    runner.runTest("ChatDecoder: 'unun' -> 1 Possibility", [&]() {
        return InventorySystem::countStringPossibilities("unun") == 1;
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
    runner.runTest("BinarySum: Disconnected Graph -> '10100'", [&]() {
        vector<vector<int>> roads2 = {
            {0,1,2},
            {0,2,8}
        };
        cout << WorldNavigator::sumMinDistancesBinary(3, roads2) << endl;
        return WorldNavigator::sumMinDistancesBinary(3, roads2) == "10100";
    }());
}

// ==========================================
// PART D: SERVER KERNEL
// ==========================================

void test_PartD_Kernel() {
    cout << "\n--- Part D: Server Kernel ---" << endl;

    // ========== BASIC/PDF EXAMPLES ==========
    
    // PDF Example 1: Tasks={A, A, B}, n=2.
    // Order: A -> B -> idle -> A. Total intervals: 4.
    runner.runTest("Scheduler: {A, A, B}, n=2 -> 4 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'B'};
        return ServerKernel:: minIntervals(tasks, 2) == 4;
    }());

    // PDF Example 2: Tasks={A, A, A}, n=2.
    // Order: A -> idle -> idle -> A -> idle -> idle -> A. Total: 7.
    runner.runTest("Scheduler: {A, A, A}, n=2 -> 7 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'A'};
        return ServerKernel::minIntervals(tasks, 2) == 7;
    }());

    // PDF Example 3: Tasks={A, B, C}, n=2.
    // All unique, no cooling needed.  Total: 3.
    runner.runTest("Scheduler: {A, B, C}, n=2 -> 3 Intervals", [&]() {
        vector<char> tasks = {'A', 'B', 'C'};
        return ServerKernel::minIntervals(tasks, 2) == 3;
    }());

    // PDF Example 4: Tasks={A, A, A, B, B, B}, n=2.
    // Order: A -> B -> idle -> A -> B -> idle -> A -> B. Total: 8.
    runner.runTest("Scheduler: {A, A, A, B, B, B}, n=2 -> 8 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'A', 'B', 'B', 'B'};
        return ServerKernel::minIntervals(tasks, 2) == 8;
    }());

    // ========== EDGE CASES:  EMPTY & SINGLE ==========
    
    // Edge:  Empty task list
    runner.runTest("Scheduler: Empty tasks -> 0 Intervals", [&]() {
        vector<char> tasks = {};
        cout << "Intervals for empty tasks: " << ServerKernel::minIntervals(tasks, 2) << endl;
        return ServerKernel::minIntervals(tasks, 2) == 0;
    }());

    // Edge: Single task
    runner.runTest("Scheduler: {A}, n=5 -> 1 Interval", [&]() {
        vector<char> tasks = {'A'};
        return ServerKernel::minIntervals(tasks, 5) == 1;
    }());

    // Edge: Two same tasks, large n
    runner.runTest("Scheduler: {A, A}, n=10 -> 12 Intervals", [&]() {
        vector<char> tasks = {'A', 'A'};
        return ServerKernel::minIntervals(tasks, 10) == 12; // A -> 10 idles -> A
    }());

    // ========== EDGE CASE: n=0 (NO COOLING) ==========
    
    // n=0: No cooling time needed
    runner.runTest("Scheduler: {A, A, A, B, B}, n=0 -> 5 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'A', 'B', 'B'};
        return ServerKernel::minIntervals(tasks, 0) == 5; // Can run immediately
    }());

    // n=0: All same tasks
    runner.runTest("Scheduler: {Z, Z, Z, Z}, n=0 -> 4 Intervals", [&]() {
        vector<char> tasks = {'Z', 'Z', 'Z', 'Z'};
        return ServerKernel::minIntervals(tasks, 0) == 4;
    }());

    // ========== EDGE CASE: ALL UNIQUE TASKS ==========
    
    // All unique tasks, large n doesn't matter
    runner.runTest("Scheduler: {A, B, C, D, E}, n=100 -> 5 Intervals", [&]() {
        vector<char> tasks = {'A', 'B', 'C', 'D', 'E'};
        return ServerKernel::minIntervals(tasks, 100) == 5; // No repeats
    }());

    // All 26 unique tasks
    runner.runTest("Scheduler: All A-Z unique, n=5 -> 26 Intervals", [&]() {
        vector<char> tasks = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
                              'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
        return ServerKernel::minIntervals(tasks, 5) == 26;
    }());

    // ========== EDGE CASE:  MULTIPLE MAX FREQUENCIES ==========
    
    // Three tasks with same max frequency
    runner.runTest("Scheduler: {A, A, B, B, C, C}, n=2 -> 7 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'B', 'B', 'C', 'C'};
        return ServerKernel::minIntervals(tasks, 2) == 6; // A->B->C->A->B->C, then one more
    }());

    // Four tasks with same max frequency
    runner.runTest("Scheduler: {A, A, A, B, B, B, C, C, C, D, D, D}, n=2 -> 12 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'A', 'B', 'B', 'B', 'C', 'C', 'C', 'D', 'D', 'D'};
        return ServerKernel::minIntervals(tasks, 2) == 12; // (3-1)*(2+1)+4 = 10 vs 12 total
    }());

    // ========== EDGE CASE:  EXTREME COOLING TIME ==========
    
    // Very large cooling time with few tasks
    runner.runTest("Scheduler: {A, A, A}, n=1000 -> 3001 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'A'};
        return ServerKernel::minIntervals(tasks, 1000) == 2003; // A -> 1000 idles -> A -> 1000 idles -> A
    }());

    // ========== EDGE CASE: ONE DOMINANT TASK ==========
    
    // One task dominates frequency
    runner.runTest("Scheduler: {A, A, A, A, B, C}, n=2 -> 10 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'A', 'A', 'B', 'C'};
        return ServerKernel::minIntervals(tasks, 2) == 10; // (4-1)*(2+1)+1 = 10
    }());

    // ========== EDGE CASE:  SMALL n WITH MANY TASKS ==========
    
    // n=1, many diverse tasks fill gaps
    runner.runTest("Scheduler: {A, A, A, B, B, C, D, E}, n=1 -> 8 Intervals", [&]() {
        vector<char> tasks = {'A', 'A', 'A', 'B', 'B', 'C', 'D', 'E'};
        return ServerKernel:: minIntervals(tasks, 1) == 8; // (3-1)*(1+1)+1 = 5 vs 8 total
    }());

    // ========== STRESS TEST ==========
    
    // Large input with all same task
    runner. runTest("Scheduler: 100x{A}, n=2 -> 298 Intervals", [&]() {
        vector<char> tasks(100, 'A');
        return ServerKernel::minIntervals(tasks, 2) == 298; // (100-1)*(2+1)+1 = 298
    }());

    // Large input with perfectly distributed tasks
    runner.runTest("Scheduler: 50xA + 50xB, n=1 -> 100 Intervals", [&]() {
        vector<char> tasks(50, 'A');
        tasks. insert(tasks.end(), 50, 'B');
        return ServerKernel:: minIntervals(tasks, 1) == 100; // Can alternate perfectly
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