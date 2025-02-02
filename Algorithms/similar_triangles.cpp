#include <cstdint>
#include <iostream>
#include <numeric>
#include <optional>
#include <random>
#include <vector>

struct Triangle {
    int first;
    int second;
    int third;
};

bool operator==(Triangle a_triangle, Triangle b_triangle) {
    return (a_triangle.first == b_triangle.first) &&
           (a_triangle.second == b_triangle.second) &&
           (a_triangle.third == b_triangle.third);
}

bool Prime(int64_t number) {
    for (int64_t i = 2; i <= static_cast<int64_t>(sqrt(number)) + 1; ++i) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

int64_t BiggerPrime(int64_t number) {
    while (true) {
        if (Prime(number)) {
            return number;
        }
        number++;
    }
}

void SidesSimplification(int& a_side, int& b_side, int& c_side) {
    int gcd = std::gcd(std::gcd(a_side, b_side), c_side);
    a_side /= gcd;
    b_side /= gcd;
    c_side /= gcd;

    int exchanger = 0;

    if (a_side > b_side) {
        exchanger = a_side;
        a_side = b_side;
        b_side = exchanger;
    }

    if (a_side > c_side) {
        exchanger = a_side;
        a_side = c_side;
        c_side = exchanger;
    }

    if (b_side > c_side) {
        exchanger = b_side;
        b_side = c_side;
        c_side = exchanger;
    }
}

class HashFunction {
private:
    int64_t parameter_;
    int64_t squared_parameter_;
    int64_t p_;
    int m_;
    std::mt19937 generator_;
    const int kSeed = 75837;

public:
    HashFunction(int64_t p_parameter, int m_parameter) {
        generator_.seed(kSeed);
        p_ = p_parameter;
        m_ = m_parameter;
        parameter_ = std::uniform_int_distribution<int64_t>(
            1, p_parameter - 1)(generator_);
        squared_parameter_ = parameter_ * parameter_;
    }

    int operator()(Triangle triangle) const {
        return static_cast<int>((static_cast<int64_t>(triangle.third) +
                                 triangle.second * parameter_ +
                                 triangle.first * squared_parameter_) %
                                p_) %
               m_;
    }
};

int ClassCounter(const std::vector<Triangle>& triangles, int number) {
    const int kHashTableSize = number;
    const int64_t kP = BiggerPrime(number);

    std::vector<std::optional<std::vector<Triangle>>> hash_table(number);

    for (int i = 0; i < number; ++i) {
        hash_table[i] = std::nullopt;
    }

    HashFunction hash_function = HashFunction{kP, kHashTableSize};

    int counter = 0;

    for (Triangle triangle : triangles) {
        int address = hash_function(triangle);
        if (!hash_table[address].has_value()) {
            hash_table[address] = {triangle};
            counter++;
        } else {
            auto& bucket = hash_table[address].value();
            bool bucket_has_triangle = false;

            for (Triangle element : bucket) {
                if (element == triangle) {
                    bucket_has_triangle = true;
                    break;
                }
            }

            if (!bucket_has_triangle) {
                bucket.push_back(triangle);
                counter++;
            }
        }
    }
    return counter;
}

int StupidFunction(const std::vector<Triangle>& triangles, int number) {
    std::vector<Triangle> counts;
    int counter = 0;

    for (Triangle triangle : triangles) {
        if (counts.empty()) {
            counts.push_back(triangle);
            counter++;
        } else {
            bool counts_has_value = false;
            for (Triangle element : counts) {
                if (element == triangle) {
                    counts_has_value = true;
                    break;
                }
            }
            if (!counts_has_value) {
                counter++;
            }
        }
    }
    return counter;
}

std::vector<Triangle> GenRandomArray(std::mt19937* gen, size_t count, int from,
                                     int to) {
    std::uniform_int_distribution<int> dist1(from, to);
    std::uniform_int_distribution<int> dist2(from, to);
    std::uniform_int_distribution<int> dist3(from, to);
    std::vector<Triangle> data(count);
    for (Triangle& cur : data) {
        cur = Triangle{dist1(*gen), dist2(*gen), dist3(*gen)};
    }
    return data;
}

void StressTest() {
    std::mt19937 generator(72874);  // NOLINT
    const int kMaxValue = 1'000'000'000;
    const int kMaxSize = 1'000;
    for (int iter = 1; iter <= 100; ++iter) {  // NOLINT
        std::cerr << "Test " << iter << "... ";
        auto data = GenRandomArray(&generator, kMaxSize, 1, kMaxValue);
        auto ok_answer = StupidFunction(data, data.size());
        auto my_answer = ClassCounter(data, data.size());
        if (ok_answer == my_answer) {
            std::cerr << "OK\n";
        } else {
            std::cerr << "Fail\n";
            for (auto cur : data) {
                std::cerr << cur.first << " " << cur.second << " " << cur.third
                          << '\n';
            }
            std::cerr << "\n";
            std::cerr << "Ok ans " << ok_answer << ", my ans " << my_answer
                      << "\n";
            break;
        }
    }
}

int main() {
    /* int number = 0;
    std::vector<Triangle> triangles;

    std::cin >> number;

    for (int i = 0; i < number; ++i) {
        int a_side = 0;
        int b_side = 0;
        int c_side = 0;
        std::cin >> a_side >> b_side >> c_side;

        if (a_side != 0|| b_side != 0 || c_side != 0) {
            SidesSimplification(a_side, b_side, c_side);
        }

        Triangle triangle = Triangle{a_side, b_side, c_side};

        triangles.push_back(triangle);
    }

    std::cout << ClassCounter(triangles, number) << '\n'; */
    StressTest();
    return 0;
}