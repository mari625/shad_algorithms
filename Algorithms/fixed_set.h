#include <math.h>

#include <cstdint>
#include <iostream>
#include <optional>
#include <random>
#include <vector>

class FixedSet {
private:
    std::vector<std::optional<std::vector<std::optional<int>>>> set_;
    int m_;
    std::mt19937 generator_;
    const int kSeed = 12;

    template <uint prime_mod>
    class HashFunction {
    private:
        uint a_parameter_;
        uint b_parameter_;

    public:
        HashFunction<prime_mod>() {}

        HashFunction<prime_mod>(std::mt19937* generator) {
            a_parameter_ = std::uniform_int_distribution<int>(
                1, prime_mod - 1)(*generator);
            b_parameter_ = std::uniform_int_distribution<int>(
                0, prime_mod - 1)(*generator);
        }

        int operator()(int key) const {
            return abs(
                static_cast<int>((a_parameter_ * static_cast<int64_t>(key) +
                                  static_cast<int64_t>(b_parameter_)) %
                                 prime_mod));
        }
    };

    static constexpr uint kPrimeMod = 1'000'000'007;
    HashFunction<kPrimeMod> big_hash_function_;
    std::vector<std::optional<HashFunction<kPrimeMod>>> small_hash_functions_;

    static int NextTwoPower(size_t number) {
        number--;
        number = number | (number >> 1);
        number = number | (number >> 2);
        number = number | (number >> 4);
        number = number | (number >> 8);   // NOLINT
        number = number | (number >> 16);  // NOLINT
        return static_cast<int>(number + 1);
    }

    void CreateBuckets(const std::vector<int>& numbers,
                       const int kBigHashTableSize) {
        const int kMaxBucketSize = numbers.size() * 10;

        set_.resize(kBigHashTableSize);
        small_hash_functions_.resize(kBigHashTableSize);

        using CurrentHashFunction = HashFunction<kPrimeMod>;

        bool true_hash_found = false;
        while (!true_hash_found) {
            for (int i = 0; i < kBigHashTableSize; ++i) {
                set_[i].reset();
                small_hash_functions_[i].reset();
            }

            big_hash_function_ = CurrentHashFunction{&generator_};
            true_hash_found = true;

            int sum_of_squared_bucket_size = 0;

            for (int i = 0; i < std::ssize(numbers); ++i) {
                int address =
                    (big_hash_function_(numbers[i])) & (kBigHashTableSize - 1);
                if (!set_[address].has_value()) {
                    set_[address] = std::vector<std::optional<int>>{numbers[i]};
                } else {
                    set_[address].value().push_back(numbers[i]);
                }

                sum_of_squared_bucket_size += set_[address]->capacity();

                if (sum_of_squared_bucket_size > kMaxBucketSize) {
                    true_hash_found = false;
                    break;
                }
            }
        }
    }

    void InsertValuesInBuckect(const std::vector<std::optional<int>>& values,
                               int index) {
        const int kBucketSize = std::pow(values.size(), 2);

        auto& bucket = set_[index].value();

        bucket.resize(kBucketSize);

        using CurrentHashFunction = HashFunction<kPrimeMod>;

        bool true_hash_found = false;
        while (!true_hash_found) {
            bucket.clear();
            for (int i = 0; i < kBucketSize; ++i) {
                bucket[i].reset();
            }

            small_hash_functions_[index] = CurrentHashFunction{&generator_};
            true_hash_found = true;

            for (int i = 0; i < std::ssize(values); ++i) {
                auto address =
                    small_hash_functions_[index].value()(values[i].value()) %
                    kBucketSize;  // i-hashfunction address in bucket hashes
                if (!(bucket[address]).has_value()) {
                    bucket[address] =
                        values[i].value();  // put the value in
                                            // i-hashfunction(key) position
                } else {
                    true_hash_found = false;
                    break;
                }
            }
        }
    }

public:
    FixedSet() { generator_.seed(kSeed); }

    void Initialize(const std::vector<int>& numbers) {
        if (numbers.empty()) {
            return;
        }

        m_ = NextTwoPower(numbers.size());

        CreateBuckets(numbers, m_);

        for (int i = 0; i < m_; ++i) {
            if (set_[i].has_value()) {
                std::vector<std::optional<int>> values = set_[i].value();
                InsertValuesInBuckect(values, i);
            }
        }
    }

    bool Contains(int number) const {
        if (set_.capacity() > 0) {
            auto index_in_big_hashtable = big_hash_function_(number) & (m_ - 1);
            const auto& bucket = set_[index_in_big_hashtable];

            if (!bucket.has_value()) {
                return false;
            }

            const auto& small_hash_function =
                small_hash_functions_[index_in_big_hashtable].value();
            auto index_in_small_hashtable =
                small_hash_function(number) % (bucket->capacity());

            auto value = bucket.value()[index_in_small_hashtable];

            return value.has_value() && value.value() == number;
        }
        return false;
    }
};