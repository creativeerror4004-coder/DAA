#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int from, to;
    double cost;
    Edge(int f, int t, double c) : from(f), to(t), cost(c) {}
};

struct Node {
    int id, stage;
    Node(int i, int s) : id(i), stage(s) {}
};

class Graph {
private:
    unordered_map<int, Node*> nodes;
    unordered_map<int, vector<Edge>> adj;
    unordered_map<int, vector<int>> stageNodes;

public:
    
    void addNode(int id, int stage) {
        nodes[id] = new Node(id, stage);
        adj[id] = {};
        stageNodes[stage].push_back(id);
    }

    
    void addEdge(int from, int to, double cost) {
        if (!nodes.count(from) || !nodes.count(to))
            throw runtime_error("Invalid node id");
        adj[from].emplace_back(from, to, cost);
    }

 
    void findMinCostPath(int sourceId, int destId) {
        if (!nodes.count(sourceId) || !nodes.count(destId) ||
            nodes[sourceId]->stage > nodes[destId]->stage) {
            cout << "Invalid source or destination\n";
            return;
        }

        unordered_map<int, double> dp;
        unordered_map<int, int> parent;

        for (auto &p : nodes)
            dp[p.first] = numeric_limits<double>::infinity();
        dp[sourceId] = 0.0;

        
        for (int st = nodes[sourceId]->stage; st <= nodes[destId]->stage; ++st) {
            auto &currStage = stageNodes[st];
            for (int u : currStage) {
                double costU = dp[u];
                if (costU == numeric_limits<double>::infinity()) continue;

                for (auto &e : adj[u]) {
                    Node* toNode = nodes[e.to];
                    if (toNode->stage < st) continue; 
                    double newCost = costU + e.cost;
                    if (newCost < dp[e.to]) {
                        dp[e.to] = newCost;
                        parent[e.to] = u;
                    }
                }
            }
        }

        
        double finalCost = dp[destId];
        if (finalCost == numeric_limits<double>::infinity()) {
            cout << "No path found.\n";
            return;
        }

        
        vector<int> path;
        for (int cur = destId; cur != sourceId; cur = parent[cur])
            path.push_back(cur);
        path.push_back(sourceId);
        reverse(path.begin(), path.end());

        cout << "Minimum cost path: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i != path.size() - 1) cout << " -> ";
        }
        cout << "\nTotal cost: " << finalCost << endl;
    }
};

int main() {
    Graph g;

    // Add nodes (stage 0 to 3)
    g.addNode(1, 0); // source
    g.addNode(2, 1);
    g.addNode(3, 1);
    g.addNode(4, 2);
    g.addNode(5, 2);
    g.addNode(6, 3); // destination

    // Add edges with cost
    g.addEdge(1, 2, 5);
    g.addEdge(1, 3, 6);
    g.addEdge(2, 4, 4);
    g.addEdge(2, 5, 7);
    g.addEdge(3, 4, 2);
    g.addEdge(3, 5, 5);
    g.addEdge(4, 6, 6);
    g.addEdge(5, 6, 4);

    // Find minimum cost route
    g.findMinCostPath(1, 6);

    return 0;
}
