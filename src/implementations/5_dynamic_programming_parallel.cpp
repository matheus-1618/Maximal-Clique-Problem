#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>

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
        graph[v - 1][u - 1] = 1;  // The graph is undirected
    }

    file.close();

    return graph;
}

bool isClique(const vector<int>& candidate, vector<vector<int>>& graph) {
    bool found = false;
    int n = candidate.size();
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph[candidate[i]][candidate[j]] == 0) {
                #pragma omp critical
                found =  false;
            }
        }
    }
    return found;
}

void FindAllMaximalCliquesDP(vector<vector<int>>& graph, vector<int>& candidates, vector<int>& currentClique, vector<int>& maximalClique, map<vector<int>, bool>& memo) {
    if (candidates.empty()) {
        if (currentClique.size() > maximalClique.size() && isClique(currentClique, graph)) {
            maximalClique = currentClique;
        }
        return;
    }

    if (memo.find(currentClique) != memo.end()) {
        if (!memo[currentClique]) {
            return;
        }
    }

    int v = candidates.back();
    candidates.pop_back();

    currentClique.push_back(v);

    vector<int> newCandidates;
    #pragma omp parallel for
    for (int u : candidates) {
        if (graph[v][u] == 1) {
            #pragma omp critical
            newCandidates.push_back(u);
        }
    }

    FindAllMaximalCliquesDP(graph, newCandidates, currentClique, maximalClique, memo);

    currentClique.pop_back();
    FindAllMaximalCliquesDP(graph, candidates, currentClique, maximalClique, memo);

    candidates.push_back(v);

    if (isClique(currentClique, graph)) {
        memo[currentClique] = true;
    } else {
        memo[currentClique] = false;
    }
}

vector<int> FindMaximalClique(vector<vector<int>>& graph) {
    int numVertex = graph.size();
    vector<int> candidates;
    #pragma omp parallel for
    for (int i = 0; i < numVertex; ++i) {
        #pragma omp critical
        candidates.push_back(i);
    }
    vector<int> currentClique;
    vector<int> maximalClique;
    map<vector<int>, bool> memo;
    FindAllMaximalCliquesDP(graph, candidates, currentClique, maximalClique, memo);
    return maximalClique;
}

int main() {
    int numVertex;
    vector<vector<int>> graph;

    graph = ReadGraph("implementations/graph.txt", numVertex);
    vector<int> maximalClique = FindMaximalClique(graph);
    sort(maximalClique.begin(), maximalClique.end(), biggerThan);
    cout << "[Implementation-Dynamic] Clique's Size: " << maximalClique.size() << " Maximal Clique: ";
    for (int v : maximalClique) {
        cout << v + 1 << " ";
    }
    cout << endl;

    return 0;
}
