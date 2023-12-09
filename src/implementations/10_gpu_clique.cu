#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/functional.h>

using namespace std;

struct bigger_than {
    __host__ __device__
    bool operator()(int a, int b) const {
        return a < b;
    }
};

typedef thrust::host_vector<int> HVec;
typedef thrust::device_vector<int> DVec;

vector<vector<int>> ReadGraph(const std::string& fileName, int& numVertex) {
    ifstream file(fileName);
    int numEdges;
    file >> numVertex >> numEdges;

    vector<vector<int>> graph(numVertex, vector<int>(numVertex, 0));

    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        file >> u >> v;
        graph[u - 1][v - 1] = 1;
        graph[v - 1][u - 1] = 1;  // O graph é não direcionado
    }

    file.close();

    return graph;
}

struct is_clique {
    vector<vector<int>>& graph;

    is_clique(vector<vector<int>>& g) : graph(g) {}

    __host__ __device__
    bool operator()(const HVec& candidate) const {
        int n = candidate.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (graph[candidate[i]][candidate[j]] == 0) {
                    return false;
                }
            }
        }
        return true;
    }
};

// Function overload for device vectors
void FindAllMaximalCliques(vector<vector<int>>& graph, DVec& candidates, DVec& currentClique, DVec& maximalClique) {
    if (candidates.empty()) {
        if (currentClique.size() > maximalClique.size() && is_clique(graph)(currentClique)) {
            maximalClique = currentClique;
        }
        return;
    }

    int v = candidates.back();
    candidates.pop_back();

    // Include vertex v in the current clique
    currentClique.push_back(v);

    // Find all maximal cliques including v
    DVec newCandidates;
    for (int u : candidates) {
        if (graph[v][u] == 1) {
            newCandidates.push_back(u);
        }
    }
    FindAllMaximalCliques(graph, newCandidates, currentClique, maximalClique);

    // Exclude vertex v from the current clique
    currentClique.pop_back();

    // Find all maximal cliques without v
    FindAllMaximalCliques(graph, candidates, currentClique, maximalClique);

    candidates.push_back(v);
}

void FindAllMaximalCliques(vector<vector<int>>& graph, HVec& candidates, HVec& currentClique, HVec& maximalClique) {
    if (candidates.empty()) {
        if (currentClique.size() > maximalClique.size() && is_clique(graph)(currentClique)) {
            maximalClique = currentClique;
        }
        return;
    }

    int v = candidates.back();
    candidates.pop_back();

    // Include vertex v in the current clique
    currentClique.push_back(v);

    // Find all maximal cliques including v
    HVec newCandidates;
    for (int u : candidates) {
        if (graph[v][u] == 1) {
            newCandidates.push_back(u);
        }
    }
    FindAllMaximalCliques(graph, newCandidates, currentClique, maximalClique);

    // Exclude vertex v from the current clique
    currentClique.pop_back();

    // Find all maximal cliques without v
    FindAllMaximalCliques(graph, candidates, currentClique, maximalClique);

    candidates.push_back(v);
}

vector<int> FindMaximalClique(vector<vector<int>>& graph) {
    int numVertex = graph.size();
    HVec candidates(numVertex);
    for (int i = 0; i < numVertex; ++i) {
        candidates[i] = i;
    }
    HVec currentClique, maximalClique;
    FindAllMaximalCliques(graph, candidates, currentClique, maximalClique);

    thrust::sort(maximalClique.begin(), maximalClique.end(), bigger_than());

    return std::vector<int>(maximalClique.begin(), maximalClique.end());
}

int main() {
    int numVertex;
    vector<vector<int>> graph;

    graph = ReadGraph("implementations/graph.txt", numVertex);
    vector<int> maximalClique = FindMaximalClique(graph);

    cout << "[Implementation-GPU] Clique's Size: " << maximalClique.size() << " Maximal Clique: ";
    for (int v : maximalClique) {
        cout << v + 1 << " ";
    }
    cout << endl;

    return 0;
}
