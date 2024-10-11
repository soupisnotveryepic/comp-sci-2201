#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class DisjointSet {
    std::vector<int> parent, rank;
public:
    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++){
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x){
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py){
            return;
        }
        if (rank[px] < rank[py]){
            parent[px] = py;
        } else if (rank[px] > rank[py]){
            parent[py] = px;
        } else {
            parent[py] = px;
            rank[px]++;
        }
    }
};

struct Edge {
    int u;
    int v;
    int build_cost;
    int destroy_cost;
    bool exists;
    Edge(int u, int v, int build_cost, int destroy_cost, bool exists) 
        : u(u), v(v), build_cost(build_cost), destroy_cost(destroy_cost), exists(exists) {}
};

int char_to_cost(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    return c - 'a' + 26;
}

bool compare_edges(const Edge& a, const Edge& b) {
    int cost_a;
    int cost_b;
    if (a.exists) {
        cost_a = -a.destroy_cost;
    } else {
        cost_a = a.build_cost;
    }

    if (b.exists) {
        cost_b = -b.destroy_cost;
    } else {
        cost_b = b.build_cost;
    }
    return cost_a < cost_b;
}

int solve(const std::vector<std::string>& country, const std::vector<std::string>& build, const std::vector<std::string>& destroy) {
    int n = country.size();
    std::vector<Edge> edges;
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            bool exists = country[i][j] == '1';
            int build_cost = char_to_cost
        (build[i][j]);
            int destroy_cost = char_to_cost
        (destroy[i][j]);
            edges.push_back(Edge(i, j, build_cost, destroy_cost, exists));
        }
    }
    
    sort(edges.begin(), edges.end(), compare_edges);
    
    DisjointSet disjoint_set(n);
    int total_cost = 0;
    std::vector<bool> used(edges.size(), false);
    
    for (size_t i = 0; i < edges.size(); i++) {
        const Edge& edge = edges[i];
        if (edge.exists && disjoint_set.find(edge.u) != disjoint_set.find(edge.v)) {
            disjoint_set.unite(edge.u, edge.v);
            used[i] = true;
        }
    }
    
    disjoint_set = DisjointSet(n);
    for (size_t i = 0; i < edges.size(); i++) {
        const Edge& edge = edges[i];
        if (disjoint_set.find(edge.u) != disjoint_set.find(edge.v)) {
            disjoint_set.unite(edge.u, edge.v);
            if (!edge.exists) {
                total_cost += edge.build_cost;
            }
            used[i] = true;
        } else if (edge.exists && !used[i]) {
            total_cost += edge.destroy_cost;
        }
    }
    
    return total_cost;
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
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
    std::string country_str, build_str, destroy_str;
    getline(std::cin, country_str, ' ');
    getline(std::cin, build_str, ' ');
    getline(std::cin, destroy_str);
    
    std::vector<std::string> country = split(country_str, ',');
    std::vector<std::string> build = split(build_str, ',');
    std::vector<std::string> destroy = split(destroy_str, ',');
    
    std::cout << solve(country, build, destroy) << std::endl;
    
    return 0;
}