#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

struct node {
    int id;
    int edges;
};

bool biggerThan(int a, int b){
    return a < b;
}

bool biggerEdges(node a, node b){
    return a.edges < b.edges;
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

vector<int> FindMaximalClique(vector<vector<int>>& graph, int numVertex) {
    vector<int> MaximalClique;
    vector<node> candidates;

    // In the begin, all nodes are possible candidates
    for (int i = 0; i < numVertex; ++i) {
        node candidate;
        candidate.id = i;
        candidate.edges = 0;
        int edges = 0;
        for (int j = 0; j < numVertex; j++){
            edges += graph[j][i];
        }
        candidate.edges = edges;
        candidates.push_back(candidate);
    }
    sort(candidates.begin(),candidates.end(),biggerEdges);
  
    while (!candidates.empty()) {
        node v = candidates.back();
        candidates.pop_back();
        bool canAdd = true;

        for (int u : MaximalClique) {
            if (graph[u][v.id] == 0) {
                canAdd = false;
                break;
            }
        }

        if (canAdd) {
            MaximalClique.push_back(v.id);
            vector<node> newCandidates;
            for (node u : candidates) {
                bool adjacentToAll = true;
                for (int c : MaximalClique) {
                    if (graph[u.id][c] == 0) {
                        adjacentToAll = false;
                        break;
                    }
                }
                if (adjacentToAll) {
                    newCandidates.push_back(u);
                }
            }
            candidates = newCandidates;
        }
    }
    return MaximalClique;
}

int main() {
    int numVertex;
    vector<vector<int>> grafo;

    grafo = ReadGraph("graph.txt", numVertex);
    vector<int> maximalClique = FindMaximalClique(grafo, numVertex);
    sort(maximalClique.begin(),maximalClique.end(),biggerThan);

    cout << "[Implementation-Heuristics] Clique's Size: "<< maximalClique.size() << " Maximal Clique: ";
    for (int v : maximalClique) {
        cout << v+1 << " ";
    }
    cout << endl;

    return 0;
}
