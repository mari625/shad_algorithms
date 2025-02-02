#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

int64_t CountTrees(int begin, int end, std::vector<int64_t>& sequence,
                   std::vector<std::vector<int64_t>>& cashe) {
    int64_t ans = 0;
    const int64_t kMod = 123'456'789;

    if (begin == end) {
        return 1;
    }

    if (begin + 1 == end) {
        if (sequence[begin] != sequence[end]) {
            return 2;
        }
        return 1;
    }

    for (int left = begin; left < end - 1; ++left) {
        int right = left + 2;
        int64_t left_count;
        int64_t right_count;

        if (sequence[left] == sequence[left + 1]) {
            continue;
        }

        if (cashe[begin][left] != -1) {
            left_count = cashe[begin][left] % kMod;
        } else {
            left_count = CountTrees(begin, left, sequence, cashe) % kMod;
        }

        if (cashe[right][end] != -1) {
            right_count = cashe[right][end] % kMod;
        } else {
            right_count = CountTrees(right, end, sequence, cashe) % kMod;
        }

        ans = (ans + (left_count * right_count) % kMod) % kMod;
    }

    if (begin < end) {
        if (cashe[begin + 1][end] != -1) {
            ans = (ans + cashe[begin + 1][end]) % kMod;
        } else {
            ans = (ans + CountTrees(begin + 1, end, sequence, cashe)) % kMod;
        }
    }

    if (sequence[end] != sequence[end - 1] && begin < end) {
        if (cashe[begin][end - 1] != -1) {
            ans = (ans + cashe[begin][end - 1]) % kMod;
        } else {
            ans = (ans + CountTrees(begin, end - 1, sequence, cashe) % kMod) %
                  kMod;
        }
    }

    cashe[begin][end] = ans;

    return ans;
}

int main() {
    int number = 0;
    std::cin >> number;

    std::vector<int64_t> sequence(number);

    for (int i = 0; i < number; ++i) {
        std::cin >> sequence[i];
    }

    std::sort(sequence.begin(), sequence.end());

    std::vector<std::vector<int64_t>> cashe(number,
                                            std::vector<int64_t>(number, -1));
    cashe[0][0] = 1;
    cashe[number - 1][number - 1] = 1;

    std::cout << CountTrees(0, number - 1, sequence, cashe) << '\n';
    return 0;
}