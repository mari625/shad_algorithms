#include <cstdint>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

struct Point {
    int x = 0;
    int y = 0;
};

struct Sizes {
    int length = 0;
    int height = 0;
};

struct SquaresAndSize {
    Point first;
    Point second;
    int size = 0;
};

class HashFunction {
private:
    int x_;
    int y_;
    int64_t p_;
    // int m_;
    std::mt19937 generator_;
    const int kSeed = 75837;

    std::vector<std::vector<int>> x_y_degrees_;
    std::vector<std::vector<int>> square_hashes_;

    /* static int64_t NextTwoPower(int64_t number) {
        number--;
        number = number | (number >> 1);
        number = number | (number >> 2);
        number = number | (number >> 4);
        number = number | (number >> 8);   // NOLINT
        number = number | (number >> 16);  // NOLINT
        return static_cast<int64_t>(number + 1);
    } */

    void PrecomputeDegrees(int length, int height) {
        x_y_degrees_.resize(length, std::vector<int>(height));
        x_y_degrees_[0][0] = 1;

        for (int i = 0; i < length; ++i) {
            if (i != 0) {
                x_y_degrees_[i][0] = static_cast<int>(
                    (static_cast<int64_t>(x_y_degrees_[i - 1][0]) *
                     static_cast<int64_t>(x_)) %
                    p_);
            }

            for (int j = 1; j < height; ++j) {
                x_y_degrees_[i][j] = static_cast<int>(
                    (static_cast<int64_t>(x_y_degrees_[i][j - 1]) *
                     static_cast<int64_t>(y_)) %
                    p_);
            }
        }
    }

public:
    HashFunction(int64_t p_parameter, int length, int heigth) {
        generator_.seed(kSeed);
        p_ = p_parameter;
        // m_ = NextTwoPower(length * heigth);
        x_ = std::uniform_int_distribution<int>(1, p_ - 1)(generator_);
        y_ = std::uniform_int_distribution<int>(1, p_ - 1)(generator_);

        PrecomputeDegrees(length, heigth);
    }

    void PrecomputeSquareHashes(int length, int height,
                                std::vector<std::vector<char>>& matrix) {
        square_hashes_.resize(length, std::vector<int>(height, 0));

        square_hashes_[0][0] = matrix[0][0] - '0';

        for (int i = 0; i < length; ++i) {
            if (i != 0) {
                square_hashes_[i][0] = static_cast<int>(
                    (static_cast<int64_t>(square_hashes_[i - 1][0]) +
                     static_cast<int64_t>(matrix[i][0] - '0') *
                         static_cast<int64_t>(x_y_degrees_[i][0])) %
                    p_);
            }

            for (int j = 1; j < height; ++j) {
                int add = square_hashes_[i][j - 1];

                if (i != 0) {
                    add += square_hashes_[i - 1][j];
                    add -= square_hashes_[i - 1][j - 1];
                }

                square_hashes_[i][j] = static_cast<int>(
                    (static_cast<int64_t>(add) +
                     static_cast<int64_t>(matrix[i][j] - '0') *
                         static_cast<int64_t>(x_y_degrees_[i][j])) %
                    p_);
            }
        }
    }

    int operator()(int i_index, int j_index) const {
        if (i_index < 0 || j_index < 0) {
            return 0;
        }

        return square_hashes_[i_index][j_index];
    }

    int operator()(int x_1, int y_1, int k_size, int length, int height) const {
        int x_2 = x_1 + k_size;
        int y_2 = y_1 + k_size;
        return static_cast<int>(
            (static_cast<int64_t>(operator()(x_2, y_2) -
                                  operator()(x_1 - 1, y_2) -
                                  operator()(x_2, y_1 - 1) +
                                  operator()(x_1 - 1, y_1 - 1) + p_ * 2) *
             static_cast<int64_t>(
                 x_y_degrees_[length - 1 - x_1]
                             [height - 1 - y_1])) %  // x_2, y_2
            p_);
        // &(m_ - 1);
    }
};

bool SquaresEqual(Point first, Point second,
                  std::vector<std::vector<char>>& matrix, int size_k) {
    for (int i = 0; i < size_k; ++i) {
        for (int j = 0; j < size_k; ++j) {
            if (matrix[first.x + i][first.y + j] !=
                matrix[second.x + i][second.y + j]) {
                return false;
            }
        }
    }
    return true;
}

bool FindSquaresForK(SquaresAndSize& answer, Sizes sizes,
                     std::vector<std::vector<char>>& matrix, int k_size,
                     HashFunction& hash_function) {
    std::unordered_map<int, std::vector<Point>> map;

    int i_iterator = 0;
    while (i_iterator <= sizes.length - k_size) {
        int j_iterator = 0;
        while (j_iterator <= sizes.height - k_size) {
            int current_hash = hash_function(i_iterator, j_iterator, k_size - 1,
                                             sizes.length, sizes.height);

            if (map[current_hash].empty()) {
                map[current_hash].push_back(Point{i_iterator, j_iterator});
            } else {
                for (int iter = 0; iter < std::ssize(map[current_hash]);
                     ++iter) {
                    if (SquaresEqual(Point{i_iterator, j_iterator},
                                     map[current_hash][iter], matrix, k_size)) {
                        answer.first = Point{i_iterator, j_iterator};
                        answer.second = map[current_hash][iter];
                        answer.size = k_size;
                        return true;
                    }
                }
                map[current_hash].push_back(Point{i_iterator, j_iterator});
            }
            ++j_iterator;
        }
        ++i_iterator;
    }
    return false;
}

SquaresAndSize FindSquares(int length, int height,
                           std::vector<std::vector<char>>& matrix) {
    SquaresAndSize answer;

    const int kBigConstant = 250007;
    HashFunction hash_function = HashFunction(kBigConstant, length, height);

    hash_function.PrecomputeSquareHashes(length, height, matrix);

    Sizes sizes = Sizes{length, height};

    int left = 1;
    int right = std::min(length, height);
    int middle = 0;
    while (left <= right) {
        middle = left + (right - left) / 2;
        if (FindSquaresForK(answer, sizes, matrix, middle, hash_function)) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }

    return answer;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int length = 0;
    int height = 0;
    std::cin >> length >> height;

    std::vector<std::vector<char>> matrix(length, std::vector<char>(height, 0));

    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < height; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    SquaresAndSize answer = FindSquares(length, height, matrix);

    if (answer.size == 0) {
        std::cout << '0' << '\n';
    } else {
        std::cout << answer.size << '\n';
        std::cout << answer.first.x + 1 << " " << answer.first.y + 1 << '\n';
        std::cout << answer.second.x + 1 << " " << answer.second.y + 1 << '\n';
    }
    return 0;
}