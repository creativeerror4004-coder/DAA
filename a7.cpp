#include <bits/stdc++.h>
using namespace std;

/*
------------------------------------------------------------
 UNIVERSITY EXAM TIMETABLE SCHEDULER (Graph Coloring)
------------------------------------------------------------
 - Courses  → Vertices
 - Conflicts → Edges (if students are common)
 - Colors (Time slots) → Exam slots with no conflicts
------------------------------------------------------------
*/

class Graph {
public:
    int V; // Number of courses
    vector<vector<int>> adj; // Adjacency list

    Graph(int v) {
        V = v;
        adj.resize(V);
    }

    // Add edge between two courses that have common students
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Display graph (for debugging)
    void printGraph() {
        cout << "\nGraph (Course conflicts):\n";
        for (int i = 0; i < V; i++) {
            cout << "Course " << i << " -> ";
            for (int x : adj[i]) cout << x << " ";
            cout << "\n";
        }
    }

    void greedyColoring(int roomLimit) {
        vector<int> result(V, -1); // color assigned to courses (-1 = uncolored)
        result[0] = 0; // first course gets first slot
        vector<bool> available(V, false);

        for (int u = 1; u < V; u++) {
            // Mark colors used by neighbors
            for (int v : adj[u]) {
                if (result[v] != -1)
                    available[result[v]] = true;
            }

            // Find first available color
            int cr;
            for (cr = 0; cr < V; cr++)
                if (available[cr] == false)
                    break;

            result[u] = cr; // Assign slot

            // Reset available[]
            for (int v : adj[u]) {
                if (result[v] != -1)
                    available[result[v]] = false;
            }
        }

        printColoring(result, "Greedy", roomLimit);
    }

    //----------------------------------------------------------
    // 2️⃣ WELSH–POWELL ALGORITHM (Order by degree)
    //----------------------------------------------------------

void welshPowell(int roomLimit) {
    vector<pair<int, int>> degree(V); // (degree, course_id)
    for (int i = 0; i < V; i++)
        degree[i] = {adj[i].size(), i};

    sort(degree.rbegin(), degree.rend()); // sort descending by degree

    vector<int> result(V, -1);
    int color = 0;

    for (auto p : degree) {
        int deg = p.first;
        int u = p.second;

        if (result[u] == -1) {
            result[u] = color;
            for (auto q : degree) {
                int v = q.second;
                if (result[v] == -1 && !isAdjacent(u, v, result))
                    result[v] = color;
            }
            color++;
        }
    }

    printColoring(result, "Welsh-Powell", roomLimit);
}


    //----------------------------------------------------------
    // 3️⃣ DSATUR ALGORITHM (Degree of Saturation)
    //----------------------------------------------------------
    void DSATUR(int roomLimit) {
        vector<int> result(V, -1); // colors
        vector<int> degree(V, 0); // degree of each vertex
        vector<set<int>> neighborColors(V); // colors used by neighbors

        for (int i = 0; i < V; i++)
            degree[i] = adj[i].size();

        int start = max_element(degree.begin(), degree.end()) - degree.begin();
        result[start] = 0;

        for (int colored = 1; colored < V; colored++) {
            
            for (int v = 0; v < V; v++) {
                if (result[v] != -1) continue;
                neighborColors[v].clear();
                for (int n : adj[v])
                    if (result[n] != -1)
                        neighborColors[v].insert(result[n]);
            }

           
            int maxSat = -1, maxDeg = -1, chosen = -1;
            for (int v = 0; v < V; v++) {
                if (result[v] == -1) {
                    int sat = neighborColors[v].size();
                    if (sat > maxSat || (sat == maxSat && degree[v] > maxDeg)) {
                        maxSat = sat;
                        maxDeg = degree[v];
                        chosen = v;
                    }
                }
            }

            // Find smallest available color
            int color = 0;
            while (neighborColors[chosen].count(color)) color++;
            result[chosen] = color;
        }

        printColoring(result, "DSATUR", roomLimit);
    }

    
    bool isAdjacent(int u, int v, vector<int> &result) {
        return find(adj[u].begin(), adj[u].end(), v) != adj[u].end();
    }

    void printColoring(vector<int> &result, string algo, int roomLimit) {
        cout << "\n------------------------------------";
        cout << "\n" << algo << " Algorithm Result";
        cout << "\n------------------------------------";
        unordered_map<int, vector<int>> slots;

        for (int i = 0; i < V; i++)
            slots[result[i]].push_back(i);

        int totalSlots = slots.size();
        cout << "\nTotal Exam Slots Used: " << totalSlots << "\n";

        // Room allocation constraint
        cout << "\nRoom Allocation (Limit " << roomLimit << " per slot):\n";
        int slotNo = 1;
        for (auto &p : slots) {
            cout << "\nSlot " << slotNo++ << " (Color " << p.first << "): ";
            int roomCount = 0;
            for (int c : p.second) {
                if (roomCount == roomLimit) {
                    cout << "\n  >> Extra courses moved to next slot";
                    roomCount = 0;
                }
                cout << "Course" << c << " ";
                roomCount++;
            }
        }
        cout << "\n------------------------------------\n";
    }
};

//--------------------------------------------------------------
// MAIN FUNCTION
//--------------------------------------------------------------
int main() {
   
    int courses = 6; // number of courses
    Graph g(courses);

   
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);

    g.printGraph();

    int roomLimit = 2; // assume only 2 rooms per slot

    g.greedyColoring(roomLimit);
    g.welshPowell(roomLimit);
    g.DSATUR(roomLimit);

    return 0;
}
