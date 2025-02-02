#include <algorithm>
#include <iostream>
#include <vector>

int GetIndexInSorted(std::vector<int>& container, int number) {
    return std::lower_bound(container.begin(), container.end(), number) -
           container.begin();
}

void AddToMask(int index, int increment, const int kMaskSize,
               std::vector<int>& mask) {
    while (index < kMaskSize) {
        mask[index] += increment;
        index = index | (index + 1);
    }
}

int GetSortedNumber(int element, std::vector<int>& mask) {
    int answer = 0;
    while (element >= 0) {
        answer += mask[element];
        element = (element & (element + 1)) - 1;
    }
    return answer;
}

int KStatisticsSortedNumber(int statistics, std::vector<int>& mask,
                            const int kMaskSize) {
    int left = -1;
    int right = kMaskSize;

    while (left + 1 < right) {
        int middle = (left + right) / 2;
        if (GetSortedNumber(middle, mask) >= statistics) {
            right = middle;
        } else {
            left = middle;
        }
    }

    return right;
}

void GetKStatistics(std::vector<int>& container,
                    std::vector<int>& sorted_container,
                    std::vector<char> commands, int k_statistics) {
    int left_iterator = 0;
    int right_iterator = 0;

    const int kMaskSize = static_cast<int>(container.size());
    std::vector<int> mask(kMaskSize);

    AddToMask(GetIndexInSorted(sorted_container, container[right_iterator]), 1,
              kMaskSize, mask);

    for (int i = 0; i < std::ssize(commands); ++i) {
        char command = commands[i];
        if (command == 'L') {
            AddToMask(
                GetIndexInSorted(sorted_container, container[left_iterator]),
                -1, kMaskSize, mask);
            left_iterator++;
        } else {
            right_iterator++;
            AddToMask(
                GetIndexInSorted(sorted_container, container[right_iterator]),
                1, kMaskSize, mask);
        }

        int statistics = KStatisticsSortedNumber(k_statistics, mask, kMaskSize);
        if (right_iterator - left_iterator + 1 < k_statistics) {
            std::cout << -1 << '\n';
        } else {
            std::cout << sorted_container[statistics] << '\n';
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int size_of_container;
    int number_of_comands;
    int k_statistics;

    std::cin >> size_of_container >> number_of_comands >> k_statistics;

    std::vector<int> container(size_of_container);
    std::vector<char> commands(number_of_comands);
    std::vector<int> sorted_container(size_of_container);

    for (int i = 0; i < size_of_container; ++i) {
        int element;
        std::cin >> element;
        container[i] = element;
        sorted_container[i] = element;
    }

    std::sort(sorted_container.begin(), sorted_container.end());

    for (int i = 0; i < number_of_comands; ++i) {
        char element;
        std::cin >> element;
        commands[i] = element;
    }

    GetKStatistics(container, sorted_container, commands, k_statistics);

    return 0;
}