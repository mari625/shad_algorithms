#include <iostream>
#include <vector>

struct Edge {
    int begin;
    int end;
    int weight;
};

struct Characteristics {
    std::vector<char> colour;
    std::vector<int> enter_time;
    std::vector<int> lambdas;
    int global_time;
    int minimal_weight;
};

void DFS(std::vector<std::vector<Edge>>& graph, Edge& vertex,
         Characteristics& info) {
    info.enter_time[vertex.end] = info.global_time++;
    info.colour[vertex.end] = 'g';
    info.lambdas[vertex.end] = info.enter_time[vertex.end];

    for (Edge& edge : graph[vertex.end]) {
        int new_vertex = edge.end;

        if (edge.end == vertex.begin) {
            continue;
        }

        if (info.colour[new_vertex] == 'g') {
            info.lambdas[vertex.end] =
                std::min(info.lambdas[vertex.end], info.enter_time[new_vertex]);
        }

        if (info.colour[new_vertex] == 'w') {
            DFS(graph, edge, info);
            info.lambdas[vertex.end] =
                std::min(info.lambdas[vertex.end], info.lambdas[new_vertex]);

            if (info.lambdas[new_vertex] > info.enter_time[vertex.end] &&
                edge.weight < info.minimal_weight) {
                info.minimal_weight = edge.weight;
            }
        }
    }

    info.colour[vertex.end] = 'b';
}

int main() {
    int number_vertex = 0;
    int number_edges = 0;

    std::cin >> number_vertex >> number_edges;

    std::vector<std::vector<Edge>> graph(number_vertex);
    for (int i = 0; i < number_edges; ++i) {
        int first = 0;
        int second = 0;
        int weight = 0;

        std::cin >> first >> second >> weight;
        graph[first - 1].push_back(Edge{first - 1, second - 1, weight});
        graph[second - 1].push_back(Edge{second - 1, first - 1, weight});
    }

    const int kBigConstant = 1'000'000'001;
    Characteristics characteristicts =
        Characteristics{std::vector<char>(number_vertex, 'w'),
                        std::vector<int>(number_vertex, 0),
                        std::vector<int>(number_vertex, 0), 0, kBigConstant};
    Edge start_edge = Edge{0, 0, 0};

    DFS(graph, start_edge, characteristicts);

    if (characteristicts.minimal_weight == kBigConstant) {
        std::cout << "-1" << '\n';
    } else {
        std::cout << characteristicts.minimal_weight << '\n';
    }

    return 0;
}