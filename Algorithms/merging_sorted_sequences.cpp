#include <math.h>

#include <iostream>
#include <vector>

std::vector<int> Merge(int first_length, int second_length,
                       std::vector<int>* first_container_ptr,
                       std::vector<int>* second_container_ptr) {
    std::vector<int> first_container = *first_container_ptr;
    std::vector<int> second_container = *second_container_ptr;

    std::vector<int> result;

    int first_iterator = 0;
    int second_iterator = 0;

    while (first_iterator < first_length && second_iterator < second_length) {
        if (first_container[first_iterator] <
            second_container[second_iterator]) {
            result.push_back(first_container[first_iterator]);
            first_iterator += 1;
        } else {
            result.push_back(second_container[second_iterator]);
            second_iterator += 1;
        }
    }

    while (first_iterator < first_length) {
        result.push_back(first_container[first_iterator]);
        first_iterator += 1;
    }
    while (second_iterator < second_length) {
        result.push_back(second_container[second_iterator]);
        second_iterator += 1;
    }

    return result;
}

std::vector<int> TotalMerge(std::vector<std::vector<int>>* large_container_ptr,
                            int number_of_containers, int length) {
    std::vector<std::vector<int>> large_container = *large_container_ptr;
    std::vector<std::vector<int>> little_container;
    int current_number = number_of_containers;
    int remained = 0;

    while (number_of_containers != 1) {
        current_number = number_of_containers + remained - 1;
        while (current_number > remained) {
            std::vector<int> merged =
                Merge(length, length, &large_container[current_number],
                      &large_container[current_number - 1]);
            large_container.pop_back();
            large_container.pop_back();
            little_container.push_back(merged);
            current_number -= 2;
        }

        for (int i = 0; i < std::ssize(little_container); ++i) {
            large_container.push_back(little_container[i]);
        }
        little_container.clear();

        if (number_of_containers % 2 == 1) {
            remained += 1;
        }
        number_of_containers = number_of_containers / 2;
        length *= 2;
    }

    while (large_container.size() != 1) {
        std::vector<int> first_container =
            large_container[large_container.size() - 1];
        std::vector<int> second_container =
            large_container[large_container.size() - 2];
        std::vector<int> merged =
            Merge(first_container.size(), second_container.size(),
                  &first_container, &second_container);
        large_container.pop_back();
        large_container.pop_back();
        large_container.push_back(merged);
    }

    return large_container[0];
}

int main() {
    int number_of_containers;
    int container_length;
    std::vector<std::vector<int>> large_container;

    std::cin >> number_of_containers >> container_length;

    for (int i = 0; i < number_of_containers; ++i) {
        std::vector<int> container;
        for (int j = 0; j < container_length; ++j) {
            int element = 0;
            std::cin >> element;
            container.push_back(element);
        }
        large_container.push_back(container);
    }

    std::vector<int> answer;

    answer =
        TotalMerge(&large_container, number_of_containers, container_length);

    for (int i = 0; i < number_of_containers * container_length; ++i) {
        std::cout << answer[i] << " ";
    }
    std::cout << '\n';

    return 0;
}
