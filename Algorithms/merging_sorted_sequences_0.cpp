#include <iostream>
#include <vector>
#include <math.h>


int BinSearch(std::vector<int>* container_ptr, int element, int begin_index, int end_index) {
    std::vector<int> container = *container_ptr;

    int left = begin_index;
    int right = end_index + 1;
    int middle = 0;

    while (left < right) {
        middle = (left + right) / 2;
        if (container[middle] < element) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return left;
}


int ExponentialBinSearch(int length, std::vector<int>* container_ptr, int element, int begin_index) {
    std::vector<int> container = *container_ptr;

    int end_index = length - 1;
    int help = 0;

    for (int i = begin_index; i < length; i = begin_index + std::pow(2, help - 1)) {
        help += 1;
        if (container[i] > element) {
            end_index = i;
            if (begin_index != 0) {
                begin_index = i - 1;
            }
            break;
        }
    }

    return BinSearch(container_ptr, element, begin_index, end_index);
}


std::vector<int> Merge(int long_length, int short_length, std::vector<int>* long_container_ptr, std::vector<int>* short_container_ptr) {
    std::vector long_container = *long_container_ptr;
    std::vector short_container = * short_container_ptr;

    int begin_index = 0;

    for (int i = 0; i < short_length; ++i) {
        int element = short_container[i];
        int position = ExponentialBinSearch(long_length, &long_container, element, begin_index);
        begin_index = position;
        long_container.insert(long_container.begin() + position, element);
        long_length += 1;
        begin_index = position;
    }

    return long_container;
}


int main() {
    int first_length;
    int second_length;
    std::vector<int> first_container;
    std::vector<int> second_container;

    std::cin >> first_length >> second_length;

    for (int i = 0; i < first_length; ++i) {
        int element = 0;
        std::cin >> element;
        first_container.push_back(element);
    }

    for (int i = 0; i < second_length; ++i) {
        int element = 0;
        std::cin >> element;
        second_container.push_back(element);
    }

    std::vector<int> answer;

    if (first_length < second_length) {
        answer = Merge(second_length, first_length, &second_container, &first_container);
    } else {
        answer = Merge(first_length, second_length, &first_container, &second_container);
    }

    for (int i = 0; i < first_length + second_length; ++i) {
        std::cout << answer[i]  << " ";
    }
    std::cout << '\n';

    return 0;
}
