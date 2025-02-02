#include <iostream>
#include <vector>

struct Edge {
    int begin;
    int end;
};

struct Categories {
    std::vector<Edge> income;
    std::vector<Edge> outcome;
};

struct Component {
    int number;
    int size = 0;
    char correct = 't';
};

void DFS(std::vector<Categories>& graph, int vertex_number,
         std::vector<char>& colour, std::vector<int>& answer) {
    colour[vertex_number] = 'g';

    for (Edge& edge : graph[vertex_number].outcome) {
        int new_vertex_number = edge.end;
        if (colour[new_vertex_number] == 'w') {
            DFS(graph, new_vertex_number, colour, answer);
        }
    }

    colour[vertex_number] = 'b';
    answer.push_back(vertex_number);
}

void SFD(std::vector<Categories>& graph, int vertex_number,
         std::vector<char>& colour, std::vector<int>& vertices_in_components,
         Component& component) {
    colour[vertex_number] = 'g';

    for (Edge& edge : graph[vertex_number].income) {
        int new_vertex_number = edge.end;
        if (colour[new_vertex_number] == 'w') {
            SFD(graph, new_vertex_number, colour, vertices_in_components,
                component);
        }
        if (colour[new_vertex_number] == 'b' &&
            vertices_in_components[new_vertex_number] != component.number) {
            component.correct = 'f';
        }
    }

    colour[vertex_number] = 'b';
    vertices_in_components[vertex_number] = component.number;
    component.size += 1;
}

int FindCompany(std::vector<Categories>& graph, int vertex_number) {
    std::vector<char> colour(vertex_number, 'w');
    std::vector<int> answer;

    const int kConst = 50000;

    for (int i = 0; i < vertex_number; ++i) {
        if (colour[i] == 'w') {
            DFS(graph, i, colour, answer);
        }
    }

    colour = std::vector<char>(vertex_number, 'w');

    std::vector<int> vertices_in_components(vertex_number, 0);
    std::vector<Component> components;
    int component_number = 0;
    int minimum = kConst;
    Component new_component;

    for (int i = vertex_number - 1; i >= 0; --i) {
        if (colour[answer[i]] == 'w') {
            component_number += 1;
            new_component = Component{component_number, 0, 't'};
            SFD(graph, answer[i], colour, vertices_in_components,
                new_component);

            if (new_component.correct == 't' && new_component.size < minimum) {
                minimum = new_component.size;
            }
        }
    }

    return vertex_number - minimum + 1;
}

int main() {
    int vertex_number = 0;
    int edge_number = 0;

    std::cin >> vertex_number >> edge_number;

    std::vector<Categories> graph(vertex_number);

    for (int i = 0; i < edge_number; ++i) {
        int first = 0;
        int second = 0;
        int result = 0;
        std::cin >> first >> second >> result;

        if (result == 1) {
            graph[first - 1].outcome.push_back(Edge{first - 1, second - 1});
            graph[second - 1].income.push_back(Edge{second - 1, first - 1});
        } else if (result == 2) {
            graph[first - 1].income.push_back(Edge{first - 1, second - 1});
            graph[second - 1].outcome.push_back(Edge{second - 1, first - 1});
        }
    }

    std::cout << FindCompany(graph, vertex_number) << '\n';

    return 0;
}