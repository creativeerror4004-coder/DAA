#include <bits/stdc++.h>
using namespace std;

// ------------------------------
// Dijkstra's Algorithm
// ------------------------------
void dijkstra(int source, vector<vector<pair<int, int>>> &graph,vector<int> &dist, vector<int> &parent){
    int n = graph.size();
    dist.assign(n, INT_MAX);
    parent.assign(n,-1);
 
    // Min-heap (priority queue)                        
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;
        // Traverse all neighbors
        for (auto &edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;  
                parent[v] = u;
                pq.push({dist[v], v});
            }

        }
    }
}

// graph[0] = (1,1), (2,4)
// graph[1] = (0,1), (2,2), (3,6)
// graph[2] = (0,4), (1,2), (3,3)
// graph[3] = (1,6), (2,3)

// ------------------------------
// Function to update traffic (edge weight)
// ------------------------------
void updateTraffic(int u, int v, int newTime, vector<vector<pair<int, int>>> &graph){
    for (auto &edge : graph[u]) {
        if (edge.first == v) {
            edge.second = newTime;
            break;
        }
    }
    for (auto &edge : graph[v]) {
        if (edge.first == u) {
            edge.second = newTime;
            break;
        }
    }
    cout << "\n Traffic updated: Road between " << u << " and " << v
         << " now takes " << newTime << " minutes.\n";
}

// ------------------------------
// Function to print the shortest path
// ------------------------------
void showPath(int dest, vector<int> &parent) {
    vector<int> path;
    for (int v = dest; v != -1; v = parent[v])
        path.push_back(v);
    reverse(path.begin(), path.end());

    cout << "  Optimal Path: ";
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i != path.size() - 1) cout << " -> ";
    }
    cout << "\n";
}

// ------------------------------
// Main Function
// ------------------------------

int main() {
    cout << "SMART TRAFFIC MANAGEMENT SYSTEM \n\n";

    int V, E;
    cout << "Enter number of intersections (nodes): ";
    cin >> V;   
    cout << "Enter number of roads (edges): ";
    cin >> E;

    vector<vector<pair<int, int>>> graph(V);

    cout << "\nEnter roads (u v time_in_minutes):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // undirected road
    }

    int source;
    cout << "\nEnter ambulance's current location (source node): ";
    cin >> source;

    int H;
    cout << "Enter number of hospitals: ";
    cin >> H;
    vector<int> hospitals(H);
    cout << "Enter hospital nodes: ";
    for (int i = 0; i < H; i++)
        cin >> hospitals[i];

    vector<int> dist, parent;

    // 1️ Run Dijkstra for initial route
    dijkstra(source, graph, dist, parent);

    // Find nearest hospital
    int nearestHospital = -1, minTime = INT_MAX;
    for (int h : hospitals) {
        if (dist[h] < minTime) {
            minTime = dist[h];
            nearestHospital = h;
        }
    }

    if (nearestHospital == -1) {
        cout << "\n No hospital reachable.\n";
        return 0;
    }

    cout << "\n Nearest Hospital: Node " << nearestHospital
         << " (Time: " << minTime << " minutes)\n";
    showPath(nearestHospital, parent);

    // 2️ Dynamic Traffic Update Simulation
    char ch;
    cout << "\nDo you want to update traffic conditions? (y/n): ";
    cin >> ch;

    if (ch == 'y' || ch == 'Y') {
        int u, v, newTime;
        cout << "Enter road (u v) and new travel time: ";
        cin >> u >> v >> newTime;

        updateTraffic(u, v, newTime, graph);

        cout << "\n Recomputing shortest path...\n";
        dijkstra(source, graph, dist, parent);

        nearestHospital = -1;
        minTime = INT_MAX;
        for (int h : hospitals) {
            if (dist[h] < minTime) {
                minTime = dist[h];
                nearestHospital = h;
            }
        }

        if (nearestHospital == -1) {
            cout << "\nNo hospital reachable after update.\n";
        } else {
            cout << "\n Updated Nearest Hospital: Node " << nearestHospital
                 << " (Time: " << minTime << " minutes)\n";
            showPath(nearestHospital, parent);
        }
    }

    cout << "\n System finished.\n";
    return 0;
}
