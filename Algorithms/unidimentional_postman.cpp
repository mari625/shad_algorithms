#include <math.h>

#include <cstdint>
#include <iostream>
#include <vector>

std::vector<unsigned int> Generate(int number, int a_parameter,
                                   int b_parameter) {
    std::vector<unsigned int> houses;
    unsigned int current = 0;
    const int kShift = 8;

    for (int i = 0; i < number; ++i) {
        current = current * a_parameter + b_parameter;
        unsigned int first_parameter = current >> kShift;
        current = current * a_parameter + b_parameter;
        int second_parameter = current >> kShift;
        houses.push_back(first_parameter << kShift ^ second_parameter);
    }

    return houses;
}

unsigned int MediansOfFives(std::vector<unsigned int>& houses, int begin_index,
                            int end_index) {
    std::vector<unsigned int> medians;

    for (int i = begin_index; i <= end_index; i = i + 5) {
        int first_iterator = i;
        while (first_iterator <= i + 4 && first_iterator <= end_index) {
            int second_iterator = first_iterator + 1;
            while (second_iterator <= i + 4 && second_iterator <= end_index) {
                if (houses[first_iterator] > houses[second_iterator]) {
                    unsigned int exchanger = houses[first_iterator];
                    houses[first_iterator] = houses[second_iterator];
                    houses[second_iterator] = exchanger;
                }
                second_iterator++;
            }
            first_iterator++;
        }
        if (end_index - i >= 2) {
            medians.push_back(houses[i + 2]);
        }
    }

    if (end_index - begin_index < 4) {
        return houses[begin_index + (end_index - begin_index) / 2];
    }

    return MediansOfFives(medians, 0, std::ssize(medians) - 1);
}

unsigned int Partition(std::vector<unsigned int>& houses, int left, int right,
                       unsigned int median) {
    for (int i = left; i <= right; ++i) {
        if (houses[i] == median) {
            unsigned int exchanger = median;
            houses[i] = houses[right];
            houses[right] = exchanger;
            break;
        }
    }

    int iterator = left - 1;

    for (int i = left; i <= right; ++i) {
        if (houses[i] <= median) {
            iterator++;
            if (iterator != i) {
                unsigned int exchanger = houses[i];
                houses[i] = houses[iterator];
                houses[iterator] = exchanger;
            }
        }
    }

    unsigned int exchanger = houses[iterator + 1];
    houses[iterator + 1] = houses[right];
    houses[right] = exchanger;

    return iterator + 1;
}

int FindMedian(std::vector<unsigned int>& houses, int begin_index,
               int end_index, int true_median_index) {
    unsigned int median = MediansOfFives(houses, begin_index, end_index);
    int median_index = Partition(houses, begin_index, end_index, median) - 1;

    while (median_index < true_median_index &&
           houses[median_index + 1] == median) {
        median_index++;
    }

    while (median_index > true_median_index &&
           houses[median_index - 1] == median) {
        median_index--;
    }

    if (median_index == true_median_index) {
        return median_index;
    }
    if (median_index < true_median_index) {
        return FindMedian(houses, median_index + 1, end_index,
                          true_median_index);
    }
    return FindMedian(houses, begin_index, median_index - 1, true_median_index);
}

uint64_t SumOfDistances(std::vector<unsigned int>* houses_ptr, int median_index,
                        int number_of_houses) {
    std::vector<unsigned int> houses = *houses_ptr;
    uint64_t summa = 0;

    for (int i = 0; i < number_of_houses; ++i) {
        if (i != median_index) {
            summa += static_cast<uint64_t>(
                std::abs(static_cast<long long>(houses[i]) -
                         static_cast<long long>(houses[median_index])));
        }
    }

    return summa;
}

int main() {
    int number_of_houses = 0;
    int a_parameter = 0;
    int b_parameter = 0;

    std::cin >> number_of_houses;
    std::cin >> a_parameter >> b_parameter;

    std::vector<unsigned int> houses =
        Generate(number_of_houses, a_parameter, b_parameter);

    int end_index = number_of_houses - 1;

    int median_index = FindMedian(houses, 0, end_index, end_index / 2);

    std::cout << SumOfDistances(&houses, median_index, number_of_houses);

    return 0;
}