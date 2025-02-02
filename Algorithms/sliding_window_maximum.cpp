#include <deque>
#include <iostream>
#include <vector>

std::vector<int> FindMaximums(std::vector<int>* numbers_ptr,
                              std::vector<char>* operations_ptr) {
    std::vector<int> numbers = *numbers_ptr;
    std::vector<char> operations = *operations_ptr;
    std::vector<int> answer;
    int left_iterator = 0;
    int right_iterator = 0;

    std::deque<int> storage;
    storage.push_back(numbers[0]);

    for (int i = 0; i < std::ssize(operations); ++i) {
        if (operations[i] == 'L') {
            int element = numbers[left_iterator];
            if (!storage.empty() && storage.front() == element) {
                storage.pop_front();
            }
            left_iterator++;
        } else {
            right_iterator++;
            while (!storage.empty() &&
                   numbers[right_iterator] > storage.back()) {
                storage.pop_back();
            }
            storage.push_back(numbers[right_iterator]);
        }
        answer.push_back(storage.front());
    }

    return answer;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int lenght = 0;
    std::vector<int> numbers;
    int number_of_operations = 0;
    std::vector<char> operations;

    std::cin >> lenght;

    for (int i = 0; i < lenght; ++i) {
        int number = 0;
        std::cin >> number;
        numbers.push_back(number);
    }

    std::cin >> number_of_operations;

    for (int i = 0; i < number_of_operations; ++i) {
        char operation;
        std::cin >> operation;
        operations.push_back(operation);
    }

    std::vector<int> answer = FindMaximums(&numbers, &operations);

    for (int i = 0; i < number_of_operations; ++i) {
        std::cout << answer[i] << " ";
    }
    std::cout << '\n';

    return 0;
}