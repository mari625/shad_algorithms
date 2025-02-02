#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

struct Coin {
    int value;
    int time;
};

bool CoinComparator(Coin first, Coin second) {
    return first.value < second.value;
}

int64_t FindCost(std::vector<Coin>* money_ptr, int begin_index,  // NOLINT
                 int end_index, int number_of_coins, int index) {
    std::vector<Coin> money = *money_ptr;

    Coin first_coin = money[begin_index];
    Coin second_coin;
    Coin last_coin;
    Coin pre_last_coin;

    const int kBigConstant = 2'000'000;
    int64_t first_near_cost = kBigConstant;
    int64_t first_far_cost = kBigConstant;
    int64_t last_near_cost = kBigConstant;
    int64_t last_far_cost = kBigConstant;

    if (begin_index == end_index) {
        return 0;
    }

    if (end_index - begin_index >= 1) {
        last_coin = money[end_index];

        int far_distance = last_coin.value - first_coin.value;
        if (((index != -1 && index == begin_index) || index == -1) &&
            begin_index != number_of_coins - 1) {
            first_far_cost = FindCost(money_ptr, begin_index + 1, end_index,
                                      number_of_coins, end_index) +
                             far_distance;
        }
        if (first_coin.time - first_far_cost < 0) {
            first_far_cost = kBigConstant;
        }

        if (((index != -1 && index == end_index) || index == -1) &&
            end_index != 0) {
            last_far_cost = FindCost(money_ptr, begin_index, end_index - 1,
                                     number_of_coins, begin_index) +
                            far_distance;
        }
        if (last_coin.time - last_far_cost < 0) {
            last_far_cost = kBigConstant;
        }
    }

    if (end_index - begin_index >= 2) {
        second_coin = money[begin_index + 1];
        int first_near_distance = second_coin.value - first_coin.value;

        if (((index != -1 && index == begin_index) || index == -1) &&
            begin_index != number_of_coins - 1) {
            first_near_cost = FindCost(money_ptr, begin_index + 1, end_index,
                                       number_of_coins, begin_index + 1) +
                              first_near_distance;
        }
        if (first_coin.time - first_near_cost < 0) {
            first_near_cost = kBigConstant;
        }

        pre_last_coin = money[end_index - 1];
        int last_near_distance = last_coin.value - pre_last_coin.value;

        if (((index != -1 && index == end_index) || index == -1) &&
            end_index != 0) {
            last_near_cost = FindCost(money_ptr, begin_index, end_index - 1,
                                      number_of_coins, end_index - 1) +
                             last_near_distance;
        }

        if (last_coin.time - last_near_cost < 0) {
            last_near_cost = kBigConstant;
        }
    }

    int64_t minimum = kBigConstant;

    if (minimum > first_near_cost) {
        minimum = first_near_cost;
    }

    if (minimum > first_far_cost) {
        minimum = first_far_cost;
    }

    if (minimum > last_near_cost) {
        minimum = last_near_cost;
    }

    if (minimum > last_far_cost) {
        minimum = last_far_cost;
    }

    return minimum;
}

int main() {
    int number_of_coins;
    std::vector<Coin> money;
    const int kBigConstant = 2'000'000;

    std::cin >> number_of_coins;

    for (int i = 0; i < number_of_coins; ++i) {
        int current_value = 0;
        int current_time = 0;
        std::cin >> current_value >> current_time;
        money.push_back(Coin{current_value, current_time});
    }

    std::sort(money.begin(), money.end(), CoinComparator);

    int64_t answer =
        FindCost(&money, 0, number_of_coins - 1, number_of_coins, -1);

    if (answer == kBigConstant) {
        std::cout << "No solution" << '\n';
    } else {
        std::cout << answer << '\n';
    }

    return 0;
}