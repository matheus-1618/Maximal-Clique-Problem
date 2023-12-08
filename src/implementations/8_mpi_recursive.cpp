#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <mpi.h>

using namespace std;

bool biggerThan(int a, int b) {
    return a < b;
}

vector<vector<int>> ReadGraph(const std::string& fileName, int& numVertex, int& numEdges) {
    ifstream file(fileName);
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
        if (currentClique.size() > maximalClique.size() && isClique(currentClique, graph)) {
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

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int numVertex, numEdges;
    vector<vector<int>> graph;

    if (rank == 0) {
        // Process 0 reads the graph and broadcasts it to all processes
        graph = ReadGraph("graph.txt", numVertex, numEdges);
    }

    MPI_Bcast(&numVertex, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numEdges, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        // Other processes allocate space for the graph
        graph.resize(numVertex, vector<int>(numVertex));
    }

    // Broadcast the graph data to all processes
    for (int i = 0; i < numVertex; ++i) {
        MPI_Bcast(graph[i].data(), numVertex, MPI_INT, 0, MPI_COMM_WORLD);
    }

    vector<int> candidates;
    for (int i = 0; i < numVertex; ++i) {
        candidates.push_back(i);
    }

    vector<int> currentClique;
    vector<int> maximalClique;
    FindAllMaximalCliques(graph, candidates, currentClique, maximalClique);

    // Gather results at Process 0
    if (rank == 0) {
        for (int sourceRank = 1; sourceRank < numProcesses; ++sourceRank) {
            int size;
            MPI_Recv(&size, 1, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vector<int> partialResult(size);
            MPI_Recv(partialResult.data(), size, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (partialResult.size() > maximalClique.size()) {
                maximalClique = partialResult;
            }
        }

        // Output the result
        sort(maximalClique.begin(), maximalClique.end(), biggerThan);
        cout << "[Implementation-Recursive] Clique's Size: " << maximalClique.size() << " Maximal Clique: ";
        for (int v : maximalClique) {
            cout << v + 1 << " ";
        }
        cout << endl;
    } else {
        // Send the result back to Process 0
        int size = maximalClique.size();
        MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(maximalClique.data(), maximalClique.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
