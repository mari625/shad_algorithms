#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>

struct Brilliant {
    int64_t weight;
    int64_t cost;
};

struct BrilliantPack {
    std::vector<int> numbers;
    int64_t total_weight = 0;
    int64_t total_cost = 0;

    bool operator==(BrilliantPack other) const {
        return total_cost == other.total_cost &&
               total_weight == other.total_weight && numbers == other.numbers;
    }
};

bool BrilliantPackComparator(BrilliantPack& first, BrilliantPack& second) {
    return first.total_weight < second.total_weight;
}

struct Iterators {
    int first_iterator = 0;
    int second_iterator_left = 0;
    int second_iterator_right = 0;
};

std::vector<BrilliantPack> AllVariants(std::vector<Brilliant>& brilliants,
                                       int number, int move) {
    std::vector<BrilliantPack> packs(std::pow(2, number));
    int power = 0;

    for (int i = 1; i <= std::pow(2, number); i = 2 * i) {
        for (int j = 0; j < std::ssize(packs); j += i) {
            if ((j / i) % 2 == 0) {
                continue;
            }

            for (int k = j; k < j + i; ++k) {
                packs[k].numbers.push_back(power + move);
                packs[k].total_cost += brilliants[power + move].cost;
                packs[k].total_weight += brilliants[power + move].weight;
            }
        }
        power += 1;
    }

    return packs;
}

void MoveRightIterator(std::vector<BrilliantPack>& first_part,
                       std::vector<BrilliantPack>& second_part,
                       std::deque<BrilliantPack>& storage, Iterators& iterators,
                       int64_t right) {
    while (iterators.second_iterator_right + 1 < std::ssize(second_part) &&
           second_part[iterators.second_iterator_right + 1].total_weight +
                   first_part[iterators.first_iterator].total_weight <=
               right) {
        iterators.second_iterator_right++;

        while (!storage.empty() &&
               second_part[iterators.second_iterator_right].total_cost >
                   storage.back().total_cost) {
            storage.pop_back();
        }

        storage.push_back(second_part[iterators.second_iterator_right]);
    }
}

void MoveLeftIterator(std::vector<BrilliantPack>& first_part,
                      std::vector<BrilliantPack>& second_part,
                      std::deque<BrilliantPack>& storage, Iterators& iterators,
                      int64_t left) {
    while (iterators.second_iterator_left < std::ssize(second_part) &&
           second_part[iterators.second_iterator_left].total_weight +
                   first_part[iterators.first_iterator].total_weight <
               left) {
        auto& element = second_part[iterators.second_iterator_left];
        if (!storage.empty() && storage.front() == element) {
            storage.pop_front();
        }

        iterators.second_iterator_left++;
    }
}

std::vector<int> Choose(std::vector<Brilliant>& brilliants, int number,
                        int64_t left, int64_t right) {
    std::vector<int> numbers;

    std::vector<BrilliantPack> first_part =
        AllVariants(brilliants, number / 2, 0);
    std::vector<BrilliantPack> second_part =
        AllVariants(brilliants, number - number / 2, number / 2);

    std::sort(first_part.begin(), first_part.end(), BrilliantPackComparator);
    std::sort(second_part.begin(), second_part.end(), BrilliantPackComparator);

    Iterators iterators =
        Iterators{static_cast<int>(first_part.size()) - 1, 0, 0};

    while (iterators.first_iterator > 0 &&
           first_part[iterators.first_iterator].total_weight > right) {
        iterators.first_iterator--;
    }

    std::deque<BrilliantPack> storage;
    storage.push_back(second_part[0]);
    std::pair<BrilliantPack, BrilliantPack> maximum;

    while (iterators.first_iterator >= 0) {
        MoveRightIterator(first_part, second_part, storage, iterators, right);
        MoveLeftIterator(first_part, second_part, storage, iterators, left);

        if (storage.empty()) {
            if (first_part[iterators.first_iterator].total_cost >= left &&
                first_part[iterators.first_iterator].total_cost <= right &&
                first_part[iterators.first_iterator].total_cost >
                    maximum.first.total_cost + maximum.second.total_cost) {
                maximum.first = first_part[iterators.first_iterator];
                maximum.second = second_part[0];
            }
        } else {
            if (storage.front().total_cost +
                    first_part[iterators.first_iterator].total_cost >
                maximum.first.total_cost + maximum.second.total_cost) {
                maximum.first = first_part[iterators.first_iterator];
                maximum.second = storage.front();
            }
        }
        iterators.first_iterator--;
    }

    if (maximum.first.total_cost + maximum.second.total_cost == 0) {
        return numbers;
    }

    for (int i = 0; i < std::ssize(maximum.first.numbers); ++i) {
        numbers.push_back(maximum.first.numbers[i]);
    }

    for (int i = 0; i < std::ssize(maximum.second.numbers); ++i) {
        numbers.push_back(maximum.second.numbers[i]);
    }

    return numbers;
}

int main() {
    int number = 0;
    int64_t left = 0;
    int64_t right = 0;

    std::cin >> number >> left >> right;

    std::vector<Brilliant> brilliants(number);
    for (int i = 0; i < number; ++i) {
        std::cin >> brilliants[i].weight >> brilliants[i].cost;
    }

    std::vector<int> answer = Choose(brilliants, number, left, right);

    if (answer.empty()) {
        std::cout << "0" << '\n';
    } else {
        std::cout << answer.size() << '\n';
        for (int i = 0; i < std::ssize(answer); ++i) {
            std::cout << answer[i] + 1 << '\n';
        }
    }

    return 0;
}