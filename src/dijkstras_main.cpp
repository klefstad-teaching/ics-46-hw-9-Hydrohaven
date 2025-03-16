#include <iostream>
#include <fstream>
#include <vector>
#include "dijkstras.h" 

using namespace std;

int main() {
    // -------------------------------
    // Test Dijkstra's Algorithm
    // -------------------------------
    cout << "Testing Dijkstra's Algorithm:" << endl;
    Graph G;
    try {
        // Assumes a file "graph.txt" exists with content like:
        // 4
        // 0 3 1
        // 3 0 3
        // 1 0 5
        // 3 1 2
        // 2 3 8
        // 1 2 3
        file_to_graph("src/small.txt", G);
        int source = 0;
        int destination = 2;
        vector<int> previous;
        vector<int> distances = dijkstra_shortest_path(G, source, previous);
        vector<int> path = extract_shortest_path(distances, previous, destination);
        
        cout << "Shortest path from " << source << " to " << destination << ":" << endl;
        print_path(path, distances[destination]);
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
