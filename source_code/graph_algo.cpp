#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <list>
#include <utility>
#include <algorithm>

const int INF = std::numeric_limits<int>::max();

// Adjacency list representation of a weighted graph
// pair<int, int> -> <neighbor_node, weight>
using Graph = std::vector<std::list<std::pair<int, int>>>;

/**
 * Implements Dijkstra's algorithm to find the shortest path from a source node to all other nodes in a graph.
 *
 * @param graph The graph represented as an adjacency list.
 * @param src The source node index.
 * @return A vector containing the shortest distance from src to each node.
 *         If a node is unreachable, the distance is INF.
 */
std::vector<int> dijkstraShortestPath(const Graph& graph, int src) {
    int V = graph.size();
    std::vector<int> dist(V, INF);

    // Priority queue to store pairs of <distance, node>
    // Ordered by smallest distance first
    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> pq;

    dist[src] = 0;
    pq.push(std::make_pair(0, src));

    while (!pq.empty()) {
        int u = pq.top().second;
        int d_u = pq.top().first;
        pq.pop();

        // If the distance popped is greater than what we already found, skip it
        // This handles the lazy deletion nature of priority_queue
        if (d_u > dist[u]) {
            continue;
        }

        // Iterate over neighbors of u
        for (const auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;

            // Relaxation step
            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push(std::make_pair(dist[v], v));
            }
        }
    }

    return dist;
}

/**
 * Reconstructs the path from source to target using the distance array and the graph.
 * Note: This is a simple backtracking reconstruction which assumes unique shortest paths or finds one of them.
 *
 * @param graph The weighted graph.
 * @param dist The array of shortest distances from source.
 * @param src The source node.
 * @param target The target node.
 * @return A vector representing the path from src to target.
 */
std::vector<int> reconstructPath(const Graph& graph, const std::vector<int>& dist, int src, int target) {
    std::vector<int> path;
    if (dist[target] == INF) {
        return path; // No path exists
    }

    int curr = target;
    path.push_back(curr);

    while (curr != src) {
        bool foundPredecessor = false;
        // Look for a neighbor v such that dist[v] + weight(v, curr) == dist[curr]
        // Since graph is directed, we actually need incoming edges to 'curr'.
        // However, if the graph is undirected or we don't have reverse adjacency, we might need to search differently.
        // For simplicity here, let's assume we iterate all nodes to find the predecessor (inefficient but works for generic struct)
        // OR we assume the graph is undirected for this specific helper.

        // Let's implement a backward search assuming we can access predecessors or iterate the graph.
        // To make it efficient in a real scenario, we would store a 'parent' array during Dijkstra.
        // Here, let's simulate the "parent" array extraction for demonstration.

        // Simulating looking for 'u' such that u -> curr exists and dist[u] + w == dist[curr]
        for (int u = 0; u < graph.size(); ++u) {
             for (const auto& edge : graph[u]) {
                 if (edge.first == curr) {
                     if (dist[u] + edge.second == dist[curr]) {
                         curr = u;
                         path.push_back(curr);
                         foundPredecessor = true;
                         break;
                     }
                 }
             }
             if (foundPredecessor) break;
        }

        if (!foundPredecessor) {
            // Should not happen if a path exists and dist is correct
            break;
        }
    }

    std::reverse(path.begin(), path.end());
    return path;
}

// Example usage wrapper for testing
void runGraphDemo() {
    int V = 9;
    Graph g(V);

    // Creating a sample graph
    // 0 - 1 (4)
    g[0].push_back({1, 4}); g[1].push_back({0, 4});
    // 0 - 7 (8)
    g[0].push_back({7, 8}); g[7].push_back({0, 8});
    // 1 - 2 (8)
    g[1].push_back({2, 8}); g[2].push_back({1, 8});
    // 1 - 7 (11)
    g[1].push_back({7, 11}); g[7].push_back({1, 11});
    // 2 - 3 (7)
    g[2].push_back({3, 7}); g[3].push_back({2, 7});
    // ... and so on

    std::cout << "Running Dijkstra from node 0..." << std::endl;
    std::vector<int> distances = dijkstraShortestPath(g, 0);

    for (int i = 0; i < V; ++i) {
        std::cout << "Distance to " << i << ": " << distances[i] << std::endl;
    }
}
