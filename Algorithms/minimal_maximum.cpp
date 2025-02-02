#include <iostream>
#include <vector>

int MinimalMaximum(std::vector<int>* a_container_ptr,
                   std::vector<int>* b_container_ptr, int length) {
    auto& a_container = *a_container_ptr;
    auto& b_container = *b_container_ptr;
    int left = 0;
    int right = length;
    int middle = 0;
    while (left < right) {
        middle = (left + right) / 2;
        if (a_container[middle] > b_container[middle]) {
            if (middle > 0 && b_container[middle - 1] < a_container[middle]) {
                right = middle;
            } else {
                break;
            }
        } else {
            if (middle < length - 1 &&
                a_container[middle + 1] <= b_container[middle]) {
                left = middle + 1;
            } else {
                break;
            }
        }
    }
    return middle + 1;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int number_of_a_containers = 0;
    int number_of_b_containers = 0;
    int length = 0;
    std::vector<std::vector<int>> a_containers;
    std::vector<std::vector<int>> b_containers;
    int number_of_queries = 0;
    std::vector<std::vector<int>> query_containers;
    std::vector<int> answer;

    std::cin >> number_of_a_containers >> number_of_b_containers >> length;
    for (int i = 0; i < number_of_a_containers; ++i) {
        std::vector<int> container;
        for (int j = 0; j < length; ++j) {
            int element = 0;
            std::cin >> element;
            container.push_back(element);
        }
        a_containers.push_back(container);
    }

    for (int i = 0; i < number_of_b_containers; ++i) {
        std::vector<int> container;
        for (int j = 0; j < length; ++j) {
            int element = 0;
            std::cin >> element;
            container.push_back(element);
        }
        b_containers.push_back(container);
    }

    std::cin >> number_of_queries;
    for (int i = 0; i < number_of_queries; ++i) {
        int number_of_a = 0;
        int number_of_b = 0;
        std::cin >> number_of_a >> number_of_b;
        answer.push_back(MinimalMaximum(&a_containers[number_of_a - 1],
                                        &b_containers[number_of_b - 1],
                                        length));
    }

    for (int i = 0; i < number_of_queries; ++i) {
        std::cout << answer[i] << "\n";
    }
}