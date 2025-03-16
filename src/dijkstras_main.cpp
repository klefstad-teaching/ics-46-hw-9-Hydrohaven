#include <iostream>
#include <vector>
#include <string>
#include "dijkstras.h"  // Ensure this header is in your include path

using namespace std;

struct TestCase {
    string filename;
    vector<int> expected;  // expected distances from source 0
};

void run_test(const TestCase& tc) {
    cout << "Testing graph file: src/" << tc.filename << endl;
    Graph G;
    try {
        file_to_graph("src/" + tc.filename, G);
    } catch (const runtime_error& e) {
        cerr << "Error loading graph from " << tc.filename << ": " << e.what() << endl;
        return;
    }
    
    int source = 0;
    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, source, previous);
    
    bool all_passed = true;
    // Compare computed distances to expected distances.
    if (G.numVertices != static_cast<int>(tc.expected.size())) {
        cout << "Mismatch in vertex count: Expected " << tc.expected.size() 
             << ", got " << G.numVertices << endl;
        all_passed = false;
    }
    
    for (int v = 0; v < G.numVertices && v < static_cast<int>(tc.expected.size()); ++v) {
        cout << "Vertex " << v << ": ";
        if (distances[v] != tc.expected[v]) {
            cout << "FAILED (Expected " << tc.expected[v] << ", got " << distances[v] << ")";
            all_passed = false;
        } else {
            cout << "PASSED (Distance = " << distances[v] << ")";
        }
        cout << "\n   Path: ";
        vector<int> path = extract_shortest_path(distances, previous, v);
        print_path(path, distances[v]);
    }
    if (all_passed)
        cout << "All tests for " << tc.filename << " passed!" << endl;
    else
        cout << "Some tests for " << tc.filename << " failed." << endl;
    
    cout << "---------------------------------------------" << endl;
}

int main() {
    // Define test cases using the file names and expected distances.
    vector<TestCase> tests = {
        {"small.txt", {0, 3, 6, 1}},
        {"medium.txt", {0, 5, 3, 12, 10, 2}},
        {"large.txt", {0, 7, 15, 5, 14, 11, 12}}
    };
    
    for (const auto& test : tests) {
        run_test(test);
    }
    
    return 0;
}
