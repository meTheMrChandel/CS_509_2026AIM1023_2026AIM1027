#include "../include/connected_components.h"
#include <queue>

CCResult find_connected_components(const AdjList& adj_list) {
    int n = static_cast<int>(adj_list.size());
    CCResult result;
    result.v_components.assign(n, -1);
    result.total_components = 0;

    std::vector<bool> visited(n, false);
    std::queue<int> q;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            int comp_id = result.total_components++;
            
            // Start BFS from vertex i
            visited[i] = true;
            result.v_components[i] = comp_id;
            q.push(i);

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (const auto& edge : adj_list[u]) {
                    int v = edge.i_destination;
                    if (!visited[v]) {
                        visited[v] = true;
                        result.v_components[v] = comp_id;
                        q.push(v);
                    }
                }
            }
        }
    }

    return result;
}
