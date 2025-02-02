#include <iostream>
#include <vector>

struct Point {
    int x = 0;
    int y = 0;
    int z = 0;
};

int SimpleSum(Point point,
              std::vector<std::vector<std::vector<int64_t>>>& tree) {
    int64_t sum = 0;

    if (point.x < 0 || point.y < 0 || point.z < 0) {
        return sum;
    }

    int i_iterator = point.x;
    while (i_iterator >= 0) {
        int j_iterator = point.y;
        while (j_iterator >= 0) {
            int k_iterator = point.z;
            while (k_iterator >= 0) {
                sum += tree[i_iterator][j_iterator][k_iterator];
                k_iterator = (k_iterator & (k_iterator + 1)) - 1;
            }
            j_iterator = (j_iterator & (j_iterator + 1)) - 1;
        }
        i_iterator = (i_iterator & (i_iterator + 1)) - 1;
    }

    return sum;
}

int64_t GetSum(Point point_1, Point point_2,
               std::vector<std::vector<std::vector<int64_t>>>& tree) {
    return SimpleSum(point_2, tree) -
           SimpleSum(Point{point_1.x - 1, point_2.y, point_2.z}, tree) -
           SimpleSum(Point{point_2.x, point_1.y - 1, point_2.z}, tree) -
           SimpleSum(Point{point_2.x, point_2.y, point_1.z - 1}, tree) +
           SimpleSum(Point{point_1.x - 1, point_1.y - 1, point_2.z}, tree) +
           SimpleSum(Point{point_1.x - 1, point_2.y, point_1.z - 1}, tree) +
           SimpleSum(Point{point_2.x, point_1.y - 1, point_1.z - 1}, tree) -
           SimpleSum(Point{point_1.x - 1, point_1.y - 1, point_1.z - 1}, tree);
}

void Update(Point point, int change,
            std::vector<std::vector<std::vector<int64_t>>>& tree) {
    int i_iterator = point.x;
    while (i_iterator < std::ssize(tree)) {
        int j_iterator = point.y;
        while (j_iterator < std::ssize(tree)) {
            int k_iterator = point.z;
            while (k_iterator < std::ssize(tree)) {
                tree[i_iterator][j_iterator][k_iterator] +=
                    static_cast<int64_t>(change);
                k_iterator = k_iterator | (k_iterator + 1);
            }
            j_iterator = j_iterator | (j_iterator + 1);
        }
        i_iterator = i_iterator | (i_iterator + 1);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int number = 0;
    std::cin >> number;

    std::vector<std::vector<std::vector<int64_t>>> tree(
        number, std::vector<std::vector<int64_t>>(
                    number, std::vector<int64_t>(number, 0)));

    char query;
    std::cin >> query;

    while (query != '3') {
        if (query == '1') {
            Point point;
            int change = 0;
            std::cin >> point.x >> point.y >> point.z >> change;

            Update(point, change, tree);
        } else {
            Point point_1;
            Point point_2;
            std::cin >> point_1.x >> point_1.y >> point_1.z >> point_2.x >>
                point_2.y >> point_2.z;

            std::cout << GetSum(point_1, point_2, tree) << '\n';
        }

        std::cin >> query;
    }

    return 0;
}