#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

double Probability(int number, int height,
                   std::vector<std::vector<double>>& ans);

double NodeProbability(int height, std::vector<std::vector<double>>& ans,
                       int index) {
    double probability = 0;

    if (index < 3) {
        return static_cast<double>(index == height);
    }

    if (height == 0 || height > index) {
        return 0;
    }

    if (ans[height][index] != -1) {
        probability += ans[height][index];
        return probability;
    }
    probability += Probability(index, height, ans);

    ans[height][index] = probability;

    return probability;
}

double Probability(int number, int height,
                   std::vector<std::vector<double>>& ans) {
    double probability = 0;

    if (number < 3) {
        return static_cast<double>(height == number);
    }

    height--;

    if (height == 0 || height > number) {
        return 0;
    }

    for (int left = 0; left < (number + 1) / 2; ++left) {
        int right = number - 1 - left;

        if (left < height && right < height) {
            continue;
        }

        double probability_left = 0;
        double probability_right = 0;
        double small_probability = 0;

        // left node has height of height and right node has height less than
        // height
        probability_left = NodeProbability(height, ans, left);
        for (int i = 0; i < height; ++i) {
            probability_right += NodeProbability(i, ans, right);
        }
        small_probability += probability_left * probability_right;

        probability_left = 0;

        // right node has height of height and left node has height less than
        // height
        probability_right = NodeProbability(height, ans, right);
        for (int i = 0; i < height; ++i) {
            probability_left += NodeProbability(i, ans, left);
        }
        small_probability += probability_left * probability_right;

        // left node and right node have height of height
        probability_left = NodeProbability(height, ans, left);
        probability_right = NodeProbability(height, ans, right);
        small_probability += probability_left * probability_right;

        if (left == right) {
            probability += small_probability * 1 / number;
        } else {
            probability += small_probability * 2 / number;
        }
    }

    ans[height + 1][number] = probability;
    return probability;
}

/* void Test(int number) {
    std::setprecision(10);
    double sum = 0;
    for (int i = 1; i <= number; ++i) {
        std::vector<std::vector<double>> ans(
            number + 1, std::vector<double>(number + 1, 0));

        std::cout << number << " " << i << "!!!!!!" << '\n';

        ans[0][0] = 1;
        ans[1][0] = 0;
        ans[1][1] = 1;

        if (number > 1) {
            ans[2][0] = 0;
            ans[2][1] = 0;
            ans[2][2] = 1;
        }

        sum += Probability(number, i, ans);
        std::cout << Probability(number, i, ans) << '\n';
    }
    std::cout << sum << " !" << '\n';
} */

int main() {
    int number;
    int height;
    const int kPrecision = 10;

    std::cin >> number >> height;

    // height++;

    if (height > number) {
        std::cout << 0 << '\n';
        return 0;
    }

    std::vector<std::vector<double>> ans(height + 1,
                                         std::vector<double>(number + 1, -1));

    ans[0][0] = 1;
    ans[1][0] = 0;
    ans[1][1] = 1;

    if (height > 1 && number > 1) {
        ans[2][0] = 0;
        ans[2][1] = 0;
        ans[2][2] = 1;
    }

    std::cout << std::setprecision(kPrecision)
              << Probability(number, height, ans) << '\n';
    /* std::cin >> number;
    Test(number); */
    return 0;
}