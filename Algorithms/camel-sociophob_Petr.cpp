#include <iostream>
#include <queue>
#include <vector>

struct Edge {
    int begin;
    int end;
    char weight;
    int way_length;
};

struct AllQueues {
    std::queue<Edge> zero_queue;
    std::queue<Edge> one_queue;
};

void PushNeighbour(Edge& neighbour, AllQueues& queues) {
    if (neighbour.weight == '0') {
        queues.zero_queue.push(neighbour);
    } else {
        queues.one_queue.push(neighbour);
    }
}

void PopVisited(std::queue<Edge>& queue, std::vector<char>& visited) {
    while (!queue.empty() && visited[queue.front().end - 1] != '0') {
        queue.pop();
    }
}

int CleanZeroes(AllQueues& queues, std::vector<char>& visited,
                std::vector<std::vector<Edge>>& graph, int last) {
    PopVisited(queues.zero_queue, visited);
    while (!queues.zero_queue.empty()) {
        Edge current_oasis = queues.zero_queue.front();
        visited[current_oasis.end - 1] = '1';

        if (current_oasis.end == last) {
            return current_oasis.way_length;
        }

        for (Edge& neighbour : graph[current_oasis.end - 1]) {
            if (visited[neighbour.end - 1] == '0') {
                neighbour.way_length = current_oasis.way_length;
                PushNeighbour(neighbour, queues);
            }
        }

        visited[current_oasis.end - 1] = '2';
        queues.zero_queue.pop();
        PopVisited(queues.zero_queue, visited);
    }

    return -1;
}

int CleanLayer(AllQueues& queues, std::vector<char>& visited,
               std::vector<std::vector<Edge>>& graph, int last) {
    Edge current_oasis;
    int layer_index = 0;

    PopVisited(queues.one_queue, visited);
    if (queues.one_queue.empty()) {
        return -1;
    }
    current_oasis = queues.one_queue.front();
    layer_index = current_oasis.way_length;

    while (current_oasis.way_length == layer_index) {
        visited[current_oasis.end - 1] = '1';

        current_oasis.way_length =
            current_oasis.way_length + (current_oasis.weight - '0');
        if (current_oasis.end == last) {
            return current_oasis.way_length;
        }

        for (Edge& neighbour : graph[current_oasis.end - 1]) {
            if (visited[neighbour.end - 1] == '0') {
                neighbour.way_length = current_oasis.way_length;
                PushNeighbour(neighbour, queues);
            }
        }

        visited[current_oasis.end - 1] = '2';
        queues.one_queue.pop();

        PopVisited(queues.one_queue, visited);
        if (queues.one_queue.empty()) {
            break;
        }
        current_oasis = queues.one_queue.front();
    }

    return -1;
}

int FindWay(int first, int last, std::vector<std::vector<Edge>>& graph) {
    std::vector<char> visited(graph.size(), '0');
    AllQueues queues;

    queues.zero_queue.push(Edge{first, first, '0', 0});

    while (!(queues.zero_queue.empty() && queues.one_queue.empty())) {
        int way_found = CleanZeroes(queues, visited, graph, last);
        if (way_found != -1) {
            return way_found;
        }

        way_found = CleanLayer(queues, visited, graph, last);
        if (way_found != -1) {
            return way_found;
        }
    }
    return -1;
}

int main() {
    int vertices_number = 0;
    int edges_number = 0;

    std::cin >> vertices_number >> edges_number;

    std::vector<std::vector<Edge>> graph(vertices_number);

    for (int i = 0; i < edges_number; ++i) {
        int begin = 0;
        int end = 0;
        char weight = 0;
        std::cin >> begin >> end >> weight;
        if (weight == '2') {
            graph[begin - 1].push_back(
                Edge{begin, static_cast<int>(graph.size()) + 1, '1', 0});
            graph.push_back(std::vector<Edge>{
                Edge{static_cast<int>(graph.size()) + 1, end, '1', 0}});
        } else {
            graph[begin - 1].push_back(Edge{begin, end, weight, 0});
        }
    }

    int queries_number = 0;
    std::cin >> queries_number;

    for (int i = 0; i < queries_number; ++i) {
        int first = 0;
        int last = 0;
        std::cin >> first >> last;

        std::cout << FindWay(first, last, graph) << '\n';
    }

    return 0;
}