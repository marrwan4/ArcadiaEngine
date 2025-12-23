/**
 * live_demo_test.cpp
 * Selected 15 Test Cases for Live Student Demonstration
 * SAFETY FIX: Includes Try-Catch and Signal Handling from main_test_v010.cpp
 */

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <csetjmp>
#include <csignal>

#include "ArcadiaEngine.h"

using namespace std;
using namespace std::chrono;

// --- SAFETY MECHANISMS (From main_test_v010.cpp) ---
jmp_buf jump_buffer;
bool in_test = false;

void segfault_handler(int signal) {
    if (in_test) {
        longjmp(jump_buffer, 1);
    }
}

extern "C" {
    PlayerTable *createPlayerTable();
    Leaderboard *createLeaderboard();
    AuctionTree *createAuctionTree();
}

class TestRunner {
    int count = 0;
    int passed = 0;
    int failed = 0;

  public:
    // SAFETY: Added try-catch and signal handling
    void assertTest(const string &testName, const function<bool()> &testBody, const string &detail = "") {
        count++;
        cout << "TEST: " << left << setw(40) << testName;

        // Register signal handlers for crashes
        signal(SIGSEGV, segfault_handler);
        signal(SIGABRT, segfault_handler);
        signal(SIGFPE, segfault_handler);

        in_test = true;

        if (setjmp(jump_buffer) == 0) {
            try {
                // TRY-CATCH BLOCK RESTORED
                bool condition = testBody();
                in_test = false;
                
                if (condition) {
                    cout << "[ PASS ]";
                    passed++;
                } else {
                    cout << "[ FAIL ]";
                    if (!detail.empty()) cout << " -> " << detail;
                    failed++;
                }
            } catch (const exception &e) {
                in_test = false;
                cout << "[ FAIL ] -> Exception: " << e.what();
                failed++;
            } catch (...) {
                in_test = false;
                cout << "[ FAIL ] -> Unknown Exception";
                failed++;
            }
        } else {
            // Jumped here from signal handler (CRASH)
            in_test = false;
            cout << "[ FAIL ] -> CRASH (Segmentation Fault)";
            failed++;
        }

        // Reset signals
        signal(SIGSEGV, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
        signal(SIGFPE, SIG_DFL);

        cout << endl;
    }

    void checkComplexity(string testName, function<void()> func, long long maxMicros, string desc) {
        count++;
		cout << "TEST: " << left << setw(40) << testName;
        auto start = high_resolution_clock::now();
        try {
            func();
            auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start).count();
            cout << "Time: " << duration << "us ";
            if (duration <= maxMicros) {
                cout << "[ PASS ]";
                passed++;
            } else {
                cout << "[ WARN ] -> Too Slow (limit " << maxMicros << ")";
                // Performance usually doesn't fail the build, but counts as warning
            }
        } catch (...) {
            cout << "[ FAIL ] -> Crashed during perf test";
            failed++;
        }
        cout << endl;
    }

    void printSummary() {
        cout << "\n==========================================" << endl;
        cout << "DEMO SUMMARY" << endl;
        cout << "==========================================" << endl;
        cout << "Passed: " << passed << " / " << count << endl;
        if (failed == 0) cout << "RESULT: PERFECT (Subject to hidden edge cases)" << endl;
        else cout << "RESULT: FAIL / PARTIAL" << endl;
    }
};

TestRunner runner;

void run_demo_safe() {
    cout << "\n=== LEVEL 1: MUST PASS (The Threshold) ===\n";
    
    // Wrapped in try-catch to prevent suite collapse
    try {
        runner.assertTest("test_insert_search_normal", []() {
            PlayerTable* t = createPlayerTable();
            t->insert(101, "Alice");
            bool res = (t->search(101) == "Alice");
            delete t;
            return res;
        }, "PlayerTable basic insert failed");

        runner.assertTest("test_add_get_top_1", []() {
            Leaderboard* b = createLeaderboard();
            b->addScore(1, 100);
            vector<int> top = b->getTopN(1);
            delete b;
            return top.size() == 1 && top[0] == 1;
        }, "Leaderboard top 1 failed");

        runner.assertTest("test_insert_no_crash", []() {
            AuctionTree* t = createAuctionTree();
            t->insertItem(1, 100);
            delete t;
            return true;
        }, "AuctionTree crash");

        runner.assertTest("test_path_exists_yes", []() {
            vector<vector<int>> edges = {{0, 1}, {1, 2}};
            return WorldNavigator::pathExists(3, edges, 0, 2);
        }, "Path finding failed");

        runner.assertTest("test_knapsack_standard", []() {
            vector<pair<int, int>> items = {{1, 10}, {2, 15}, {3, 40}};
            return InventorySystem::maximizeCarryValue(3, items) == 40;
        }, "Knapsack logic error");
    } catch (...) {
        cout << "[CRITICAL ERROR] Level 1 Suite Crashed\n";
    }

    cout << "\n=== LEVEL 2: CORE LOGIC ===\n";
    try {
        runner.assertTest("test_collision_handling", []() {
            PlayerTable* t = createPlayerTable();
            t->insert(10, "Col1"); t->insert(111, "Col2"); 
            bool res = (t->search(10) == "Col1" && t->search(111) == "Col2");
            delete t;
            return res;
        }, "Hashing collision failed");

        runner.assertTest("test_order_correctness", []() {
            Leaderboard* b = createLeaderboard();
            b->addScore(2, 200); b->addScore(3, 50);
            vector<int> top = b->getTopN(3);
            delete b;
            return top[0] == 2;
        }, "Leaderboard sorting failed");

        runner.assertTest("test_loot_perfect_split", []() {
            vector<int> c = {2, 2};
            return InventorySystem::optimizeLootSplit(2, c) == 0;
        }, "Loot split failed");

        runner.assertTest("test_mst_simple", []() {
            vector<vector<int>> roads = {{0, 1, 10, 0}, {1, 2, 20, 0}, {0, 2, 100, 0}};
            return WorldNavigator::minBribeCost(3, 3, 1, 1, roads) == 30;
        }, "MST failed");

        runner.assertTest("test_scheduler_basic", []() {
            vector<char> tasks = {'A', 'A', 'B'};
            return ServerKernel::minIntervals(tasks, 2) == 4;
        }, "Scheduler failed");
    } catch (...) {
        cout << "[CRITICAL ERROR] Level 2 Suite Crashed\n";
    }

    cout << "\n=== LEVEL 3: EDGE CASES (Partial Credit) ===\n";
    try {
        runner.assertTest("test_tie_breaking", []() {
            Leaderboard* b = createLeaderboard();
            b->addScore(10, 500); b->addScore(1, 500);
            vector<int> top = b->getTopN(2);
            delete b;
            return top[0] == 1; // Smaller ID first
        }, "Tie breaking incorrect");

        runner.assertTest("test_insert_duplicate_price", []() {
            AuctionTree* t = createAuctionTree();
            t->insertItem(1, 100); t->insertItem(5, 100);
            delete t; 
            return true; // Should not crash/hang
        }, "Duplicate price issue");

        runner.assertTest("test_knapsack_item_too_heavy", []() {
            vector<pair<int, int>> items = {{10, 100}};
            return InventorySystem::maximizeCarryValue(5, items) == 0;
        }, "Heavy item check failed");

        runner.assertTest("test_path_no_edges", []() {
            vector<vector<int>> edges = {};
            return !WorldNavigator::pathExists(2, edges, 0, 1);
        }, "No edges check failed");

        runner.checkComplexity("test_perf_insert_1000", []() {
            PlayerTable* t = createPlayerTable();
            for(int i=2000; i<3000; i++) t->insert(i, "P");
            delete t;
        }, 5000, "O(1)");
    } catch (...) {
        cout << "[CRITICAL ERROR] Level 3 Suite Crashed\n";
    }
}

int main() {
    run_demo_safe();
    runner.printSummary();
    return 0;
}