#include <iostream>
#include <vector>
#include <queue>
#include <cstring>


class HopcroftKarp {
private:
    int16_t n, m; // Количество вершин в каждой части двудольного графа
    std::vector<std::vector<int16_t>> adj; // Список смежности
    std::vector<int16_t> pairU, pairV, dist;

    std::vector<std::vector<int16_t>> pair;

    bool bfs() {
        std::queue<int16_t> Q;
        for (int16_t u = 0; u < n; u++) {
            if (pairU[u] == -1) {
                dist[u] = 0;
                Q.push(u);
            } else {
                dist[u] = -1;
            }
        }
        bool found = false;
        while (!Q.empty()) {
            int16_t u = Q.front();
            Q.pop();
            for (int16_t v : adj[u]) {
                if (pairV[v] == -1) {
                    found = true;
                } else if (dist[pairV[v]] == -1) {
                    dist[pairV[v]] = dist[u] + 1;
                    Q.push(pairV[v]);
                }
            }
        }
        return found;
    }

    bool dfs(int16_t u) {
        for (int16_t v : adj[u]) {
            if (pairV[v] == -1 || (dist[pairV[v]] == dist[u] + 1 && dfs(pairV[v]))) {
                if(pairU[u] == -1) {
                    pair.push_back({u, v});
                }
                // std::cout << "Debug: " << pairV[v] << "." << pairU[u];
                pairU[u] = v;
                pairV[v] = u;
                // std::cout << " Debug: " << pairV[v] << "." << pairU[u] << std::endl;
                
                
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

public:
    HopcroftKarp(int n, int m) : n(n), m(m), adj(n), pairU(n, -1), pairV(m, -1), dist(n) {}

    void addEdge(int16_t u, int16_t v) {
        adj[u].push_back(v);
    }

    int16_t maximumMatching() {
        int16_t matching = 0;
        while (bfs()) {
            for (int u = 0; u < n; u++) {
                if (pairU[u] == -1 && dfs(u)) {
                    matching++;
                }
            }
        }
        return matching;
    }

    std::vector<std::vector<int16_t>> getPairs() { return pair; }
};

int testHK() {
    int n, m; // Количество вершин в каждой части
    std::cout << "Введите количество вершин в первой части и второй части: ";
    std::cin >> n >> m;

    HopcroftKarp hk(n, m);
    
    std::cout << "Введите количество рёбер: ";
    int edges;
    std::cin >> edges;

    std::cout << "Введите рёбра (u v):" << std::endl;
    for (int i = 0; i < edges; i++) {
        int u, v;
        std::cin >> u >> v; // Вводим рёбра между вершинами
        hk.addEdge(u, v);
    }

    std::cout << "Максимальное паросочетание: " << hk.maximumMatching() << std::endl;

    std::vector<std::vector<int16_t>> pairs = hk.getPairs();
    std::cout << "Пары:";
    for(size_t index = 0; index < pairs.size(); index++) {
        std::cout << " (" << pairs[index][0] << "," << pairs[index][1] << ")" << std::endl;
    }

    return 0;
}
