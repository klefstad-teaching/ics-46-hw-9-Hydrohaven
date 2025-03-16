#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <stack>
#include <algorithm>
#include "dijkstras.h"  // Adjust the header file name if needed

using namespace std;

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    // Initialize distances to INF and previous to -1
    vector<int> dist(n, INF);
    previous.assign(n, -1);

    // Priority queue to select the vertex with the smallest tentative distance
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Skip outdated queue entries
        if (d > dist[u])
            continue;

        // Relaxation step for each edge from vertex u
        for (const Edge& e : G[u]) {
            int v = e.dst;
            int weight = e.weight;
            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                previous[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;
    // If the destination is unreachable, return an empty path
    if (distances[destination] == INF)
        return path;

    // Reconstruct the path backwards from destination
    for (int at = destination; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& v, int total) {
    // Print the path in the format: vertex0 -> vertex1 -> ... -> vertexN (Total cost: total)
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i != v.size() - 1)
            cout << " -> ";
    }
    cout << " (Total cost: " << total << ")" << endl;
}
