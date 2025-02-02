#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
    int first;
    int second;
    int weight;
};

bool EdgeComparator(Edge& first, Edge& second) {
    return first.weight < second.weight;
}

int FindTop(std::vector<int>& graph, int agent) {
    int top = agent;
    while (graph[top] != -1) {
        top = graph[top];
    }

    if (top != agent) {
        int current_agent = agent;
        while (graph[current_agent] != top) {
            current_agent = graph[agent];
            graph[agent] = top;
            agent = current_agent;
        }
    }

    return top;
}

bool MakeTop(std::vector<int>& graph, std::vector<int>& ranks, int first,
             int second) {
    int first_top = FindTop(graph, first);
    int second_top = FindTop(graph, second);

    if (first_top == second_top) {
        return false;
    }

    if (ranks[first_top] < ranks[second_top]) {
        graph[first_top] = second_top;
        return true;
    }

    graph[second_top] = first_top;
    if (ranks[first_top] == ranks[second_top]) {
        ranks[first_top] += 1;
    }

    return true;
}

int FindMin(std::vector<Edge>& edges, int vertex_number) {
    std::vector<int> ranks(vertex_number, 1);
    std::vector<int> graph(vertex_number, -1);
    int min_max_weight = 0;

    for (Edge& edge : edges) {
        bool edge_added = MakeTop(graph, ranks, edge.first, edge.second);
        if (edge_added && min_max_weight < edge.weight) {
            min_max_weight = edge.weight;
        }
    }

    return min_max_weight;
}

int main() {
    int vertex_number = 0;
    int edge_number = 0;
    std::cin >> vertex_number >> edge_number;

    std::vector<Edge> edges(edge_number);

    for (int i = 0; i < edge_number; ++i) {
        std::cin >> edges[i].first >> edges[i].second >> edges[i].weight;
        edges[i].first -= 1;
        edges[i].second -= 1;
    }

    std::sort(edges.begin(), edges.end(), EdgeComparator);

    std::cout << FindMin(edges, vertex_number) << '\n';

    return 0;
}
