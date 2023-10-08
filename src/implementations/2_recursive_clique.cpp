#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

bool biggerThan(int a, int b) {
    return a < b;
}

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

bool isClique(vector<int>& candidate, vector<vector<int>>& graph) {
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

void FindAllMaximalCliques(vector<vector<int>>& graph, vector<int>& candidates, vector<int>& currentClique, vector<int>& maximalClique) {
    if (candidates.empty()) {
        if (currentClique.size() > maximalClique.size()) {
            maximalClique = currentClique;
        }
        return;
    }

    int v = candidates.back();
    candidates.pop_back();

    // Include vertex v in the current clique
    currentClique.push_back(v);

    // Find all maximal cliques including v
    vector<int> newCandidates;
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
    vector<int> candidates;
    for (int i = 0; i < numVertex; ++i) {
        candidates.push_back(i);
    }
    vector<int> currentClique;
    vector<int> maximalClique;
    FindAllMaximalCliques(graph, candidates, currentClique, maximalClique);
    return maximalClique;
}

int main() {
    int numVertex;
    vector<vector<int>> graph;

    graph = ReadGraph("implementations/graph.txt", numVertex);
    vector<int> maximalClique = FindMaximalClique(graph);
    sort(maximalClique.begin(),maximalClique.end(),biggerThan);
    cout << "[Implementation-Recursive] Clique's Size: " << maximalClique.size() << " Maximal Clique: ";
    for (int v : maximalClique) {
        cout << v + 1 << " ";
    }
    cout << endl;

    return 0;
}
