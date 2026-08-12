#include "../include/betweenness_centrality.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <stack>
#include <vector>

static bool is_unweighted_graph(const AdjList& adj_list) {
    for (const auto& neighbors : adj_list) {
        for (const auto& edge : neighbors) {
            if (edge.i_weight != 1) {
                return false;
            }
        }
    }
    return true;
}

BCResult compute_betweenness_centrality(const AdjList& adj_list) {
    int n = static_cast<int>(adj_list.size());
    BCResult result;
    result.v_centrality.assign(n, 0.0);

    if (n == 0) {
        return result;
    }

    bool unweighted = is_unweighted_graph(adj_list);

    for (int s = 0; s < n; ++s) {
        std::vector<std::vector<int>> pred(n);
        std::vector<double> sigma(n, 0.0);
        std::vector<double> dist(n, std::numeric_limits<double>::infinity());
        std::stack<int> S;

        sigma[s] = 1.0;
        dist[s] = 0.0;

        if (unweighted) {
            std::queue<int> q;
            q.push(s);

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                S.push(u);

                for (const auto& edge : adj_list[u]) {
                    int v = edge.i_destination;
                    double path_dist = dist[u] + 1.0;

                    if (dist[v] == std::numeric_limits<double>::infinity()) {
                        dist[v] = path_dist;
                        q.push(v);
                    }

                    if (dist[v] == path_dist) {
                        sigma[v] += sigma[u];
                        pred[v].push_back(u);
                    }
                }
            }
        } else {
            using DistPair = std::pair<double, int>;
            std::priority_queue<DistPair, std::vector<DistPair>, std::greater<DistPair>> pq;
            pq.push({0.0, s});

            while (!pq.empty()) {
                DistPair top = pq.top();
                pq.pop();

                double dist_u = top.first;
                int u = top.second;

                if (dist_u > dist[u]) {
                    continue;
                }

                S.push(u);

                for (const auto& edge : adj_list[u]) {
                    int v = edge.i_destination;
                    double path_dist = dist_u + static_cast<double>(edge.i_weight);

                    if (path_dist < dist[v]) {
                        dist[v] = path_dist;
                        sigma[v] = sigma[u];
                        pred[v].clear();
                        pred[v].push_back(u);
                        pq.push({dist[v], v});
                    } else if (path_dist == dist[v]) {
                        sigma[v] += sigma[u];
                        pred[v].push_back(u);
                    }
                }
            }
        }

        std::vector<double> delta(n, 0.0);
        while (!S.empty()) {
            int w = S.top();
            S.pop();

            for (int v : pred[w]) {
                if (sigma[w] != 0.0) {
                    delta[v] += (sigma[v] / sigma[w]) * (1.0 + delta[w]);
                }
            }

            if (w != s) {
                result.v_centrality[w] += delta[w];
            }
        }
    }

    // Undirected graphs count each pair twice across all sources.
    for (double& value : result.v_centrality) {
        value *= 0.5;
    }

    return result;
}
