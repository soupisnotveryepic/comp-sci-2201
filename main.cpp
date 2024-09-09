#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class DisjointSet {
    vector<int> parent, rank;
public:
    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }
    
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rank[px] < rank[py])
            parent[px] = py;
        else if (rank[px] > rank[py])
            parent[py] = px;
        else {
            parent[py] = px;
            rank[px]++;
        }
    }
};

struct Edge {
    int u, v, cost;
    bool exists;
    Edge(int u, int v, int cost, bool exists) : u(u), v(v), cost(cost), exists(exists) {}
};

int charToCost(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    return c - 'a' + 26;
}

bool compareEdges(const Edge& a, const Edge& b) {
    if (a.exists && !b.exists) return true;
    if (!a.exists && b.exists) return false;
    return a.cost < b.cost;
}

int solve(const vector<string>& country, const vector<string>& build, const vector<string>& destroy) {
    int n = country.size();
    vector<Edge> edges;
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            bool exists = country[i][j] == '1';
            int build_cost = charToCost(build[i][j]);
            int destroy_cost = charToCost(destroy[i][j]);
            
            if (exists) {
                edges.push_back(Edge(i, j, destroy_cost, true));
            } else {
                edges.push_back(Edge(i, j, build_cost, false));
            }
        }
    }
    
    sort(edges.begin(), edges.end(), compareEdges);
    
    DisjointSet ds(n);
    int total_cost = 0;
    int edges_added = 0;
    
    for (const Edge& edge : edges) {
        if (ds.find(edge.u) != ds.find(edge.v)) {
            ds.unite(edge.u, edge.v);
            if (!edge.exists) {
                total_cost += edge.cost;  // Cost to build a new road
            }
            edges_added++;
        } else if (edge.exists) {
            total_cost += edge.cost;  // Cost to destroy an existing road
        }
        
        if (edges_added == n - 1) break;
    }
    
    return total_cost;
}

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == delimiter) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += s[i];
        }
    }
    tokens.push_back(token);
    return tokens;
}

int main() {
    string country_str, build_str, destroy_str;
    getline(cin, country_str, ' ');
    getline(cin, build_str, ' ');
    getline(cin, destroy_str);
    
    vector<string> country = split(country_str, ',');
    vector<string> build = split(build_str, ',');
    vector<string> destroy = split(destroy_str, ',');
    
    cout << solve(country, build, destroy) << endl;
    
    return 0;
}