#include <cstdint>
#include <iostream>
#include <vector>

struct Part {
    int left;
    int right;
};

struct Borders {
    int64_t left;
    int64_t right;
};

void Merge(std::vector<int64_t>& container, Part first_part, Part second_part,
           std::vector<int64_t>& scratch_space) {
    int first_iterator = first_part.left;
    int second_iterator = second_part.left;
    int third_iterator = 0;

    while (first_iterator <= first_part.right &&
           second_iterator <= second_part.right &&
           first_iterator < std::ssize(container) &&
           second_iterator < std::ssize(container)) {
        if (container[first_iterator] < container[second_iterator]) {
            scratch_space[third_iterator] = container[first_iterator];
            first_iterator += 1;
            third_iterator += 1;
        } else {
            scratch_space[third_iterator] = container[second_iterator];
            second_iterator += 1;
            third_iterator += 1;
        }
    }

    while (first_iterator <= first_part.right) {
        scratch_space[third_iterator] = container[first_iterator];
        first_iterator += 1;
        third_iterator += 1;
    }

    while (second_iterator <= second_part.right) {
        scratch_space[third_iterator] = container[second_iterator];
        second_iterator += 1;
        third_iterator += 1;
    }
}

void FindLeftIntervals(std::vector<int64_t>& partial_sums,
                       const std::vector<Part>& parts, int64_t border,
                       int64_t& counter) {
    int first_iterator = parts[0].left;
    int second_iterator = parts[1].left;

    while (first_iterator <= parts[0].right) {
        while (second_iterator <= parts[1].right) {
            if (border <=
                partial_sums[second_iterator] - partial_sums[first_iterator]) {
                counter += parts[1].right - second_iterator + 1;
                break;
            }
            second_iterator++;
        }
        first_iterator++;
    }
}

void FindRightIntervals(std::vector<int64_t>& partial_sums,
                        const std::vector<Part>& parts, int64_t border,
                        int64_t& counter) {
    int first_iterator = parts[0].left;
    int second_iterator = parts[1].left;

    while (first_iterator <= parts[0].right) {
        while (second_iterator <= parts[1].right) {
            if (border + 1 <=
                partial_sums[second_iterator] - partial_sums[first_iterator]) {
                counter -= (parts[1].right - second_iterator + 1);
                break;
            }
            second_iterator++;
        }
        first_iterator++;
    }
}

void FindSegments(std::vector<int64_t>& partial_sums,
                  std::vector<int64_t>& scratch_space, Part part,
                  Borders borders, int64_t& counter) {
    if (part.left != part.right) {
        int devider = part.left + (part.right - part.left) / 2;
        Part first_part = {part.left, devider};
        Part second_part = {devider + 1, part.right};
        std::vector<Part> parts = {first_part, second_part};

        FindSegments(partial_sums, scratch_space, first_part, borders, counter);
        FindSegments(partial_sums, scratch_space, second_part, borders,
                     counter);

        FindLeftIntervals(partial_sums, parts, borders.left, counter);
        FindRightIntervals(partial_sums, parts, borders.right, counter);

        Merge(partial_sums, first_part, second_part, scratch_space);

        for (int i = 0; i <= part.right - part.left; ++i) {
            partial_sums[part.left + i] = scratch_space[i];
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int number_of_digits = 0;
    int64_t left;
    int64_t right;
    int64_t counter = 0;

    std::cin >> number_of_digits;

    std::vector<int64_t> container(number_of_digits + 1);
    container[0] = 0;

    for (int i = 0; i < number_of_digits; ++i) {
        int64_t element;
        std::cin >> element;
        container[i + 1] = container[i] + element;
    }

    std::cin >> left >> right;
    Borders borders = {left, right};

    std::vector<int64_t> scratch_space(number_of_digits + 1);

    Part part = {0, number_of_digits};

    FindSegments(container, scratch_space, part, borders, counter);

    std::cout << counter << '\n';

    return 0;
}