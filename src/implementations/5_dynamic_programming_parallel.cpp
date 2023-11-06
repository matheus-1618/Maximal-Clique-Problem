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
    bool found = false;
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph[candidate[i]][candidate[j]] == 0) {
                #pragma omp critical
                found =  true;
            }
        }
    }
    return found;
}

vector<vector<int>> FindAllMaximalCliques(vector<vector<int>>& graph, vector<int>& candidates, vector<int>& currentClique) {
    vector<vector<int>> maximalCliques;

    if (candidates.empty()) {
        maximalCliques.push_back(currentClique);
        return maximalCliques;
    }

    int v = candidates.back();
    candidates.pop_back();

    // Include vertex v in the current clique
    currentClique.push_back(v);

    // Find all maximal cliques including v
    vector<int> newCandidates;
    #pragma omp parallel for
    for (int u : candidates) {
        if (graph[v][u] == 1) {
            #pragma omp critical
            newCandidates.push_back(u);
        }
    }
    vector<vector<int>> cliquesWithV = FindAllMaximalCliques(graph, newCandidates, currentClique);

    // Exclude vertex v from the current clique
    currentClique.pop_back();

    // Find all maximal cliques without v
    vector<vector<int>> cliquesWithoutV = FindAllMaximalCliques(graph, candidates, currentClique);

    // Combine cliques with v and without v
    #pragma omp parallel for
    for (const auto& clique : cliquesWithV) {
        #pragma omp critical
        maximalCliques.push_back(clique);
    }
    #pragma omp parallel for
    for (const auto& clique : cliquesWithoutV) {
        #pragma omp critical
        maximalCliques.push_back(clique);
    }

    candidates.push_back(v);

    return maximalCliques;
}

int main() {
    int numVertex;
    vector<vector<int>> graph;

    graph = ReadGraph("implementations/graph.txt", numVertex);
    vector<int> candidates;
    #pragma omp parallel for
    for (int i = 0; i < numVertex; ++i) {
        #pragma omp critical
        candidates.push_back(i);
    }
    vector<int> currentClique;
    vector<vector<int>> maximalCliques = FindAllMaximalCliques(graph, candidates, currentClique);

    size_t maxSize = 0;
    vector<int> largestClique;
    #pragma omp parallel for
    for (const auto& clique : maximalCliques) {
        if (clique.size() > maxSize) {
            #pragma omp critical
            maxSize = clique.size();
            largestClique = clique;
        }
    }

    sort(largestClique.begin(), largestClique.end(), biggerThan);

    cout << "[Implementation-Dynamic Parallel] Clique's Size: " << maxSize << " Maximal Clique: ";
    for (int v : largestClique) {
        cout << v + 1 << " ";
    }
    cout << endl;

    return 0;
}
