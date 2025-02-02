#include <iostream>
#include <string>
#include <vector>

int FindDirector(std::vector<int>& graph, int worker) {
    int director = worker;
    while (graph[director] != 0) {
        director = graph[director];
    }

    if (director != worker) {
        int current_worker = worker;
        while (graph[current_worker] != director) {
            current_worker = graph[worker];
            graph[worker] = director;
            worker = current_worker;
        }
    }

    return director;
}

int MakeDirector(std::vector<int>& graph, int head, int worker) {
    if (graph[worker] == 0 && FindDirector(graph, head) != worker) {
        graph[worker] = head;
        return 1;
    }

    return 0;
}

int main() {
    int human_number = 0;
    int query_number = 0;
    std::string numbers;

    std::getline(std::cin, numbers);

    auto empty = numbers.find(' ');
    if (empty != std::string::npos) {
        human_number = std::stoi(numbers.substr(0, empty));
        query_number = std::stoi(numbers.substr(empty + 1));
    }

    std::vector<int> graph(human_number + 1, 0);

    for (int i = 0; i < query_number; ++i) {
        std::string query;
        std::getline(std::cin, query);

        int worker = 0;
        int head = 0;
        auto empty = query.find(' ');
        if (empty != std::string::npos) {
            head = std::stoi(query.substr(0, empty));
            worker = std::stoi(query.substr(empty + 1));

            std::cout << MakeDirector(graph, head, worker) << '\n';
        } else {
            worker = std::stoi(query);

            std::cout << FindDirector(graph, worker) << '\n';
        }
    }
    return 0;
}