#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

struct Dot {
    int x;
    int y;
};

struct Edge {
    int first;
    int second;
    double weight;
};

bool EdgeComparator(Edge& first, Edge& second) {
    return first.weight < second.weight;
}

std::vector<Edge> CountDistances(std::vector<Dot>& dots, int dots_number) {
    std::vector<Edge> edges;

    int position = dots_number - 1;
    while (position != 0) {
        for (int i = 0; i < position; ++i) {
            edges.push_back(
                Edge{i, position,
                     std::pow(dots[i].x - dots[position].x, 2) +
                         std::pow(dots[i].y - dots[position].y, 2)});
        }

        position -= 1;
    }

    return edges;
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

double FindMin(std::vector<Edge>& edges, int agent_number) {
    std::vector<int> ranks(agent_number, 1);
    std::vector<int> graph(agent_number, -1);
    double min_max_weight = 0;

    for (Edge& edge : edges) {
        bool edge_added = MakeTop(graph, ranks, edge.first, edge.second);
        if (edge_added && min_max_weight < edge.weight) {
            min_max_weight = edge.weight;
        }
    }

    return min_max_weight;
}

int main() {
    int agent_number = 0;
    std::cin >> agent_number;

    std::vector<Dot> dots(agent_number);

    for (int i = 0; i < agent_number; ++i) {
        std::cin >> dots[i].x >> dots[i].y;
    }

    std::vector<Edge> edges = CountDistances(dots, agent_number);
    std::sort(edges.begin(), edges.end(), EdgeComparator);

    const int kPrecision = 15;

    std::cout << std::setprecision(kPrecision)
              << std::sqrt(FindMin(edges, agent_number)) << '\n';

    return 0;
}