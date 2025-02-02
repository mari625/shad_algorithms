#include <algorithm>
#include <iostream>
#include <vector>

const int kN = 1e5 + 100;
int fenw[kN];

void Add(int ind, int val) {
    for (; ind < kN; ind |= (ind + 1)) {
        fenw[ind] += val;
    }
}

int Get(int right) {
    int sum = 0;
    for (; right >= 0; right = (right & (right + 1)) - 1) {
        sum += fenw[right];
    }
    return sum;
}


int Kth(int kth) {
    int left = -1;
    int right = kN - 1;
    while (right - left > 1) {
        int tm = (left + right) / 2;
        if (Get(tm) >= kth) {
            right = tm;
        } else {
            left = tm;
        }
    }
    return right;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n_elem;
    int m_elem;
    int kth;
    std::cin >> n_elem >> m_elem >> kth;

    std::vector<int> arr(n_elem);
    std::vector<int> sorted;

    for (int i = 0; i < n_elem; ++i) {
        std::cin >> arr[i];
        sorted.push_back(arr[i]);
    }
    std::sort(sorted.begin(), sorted.end());

    auto index = [&](int val) {
        return std::lower_bound(sorted.begin(), sorted.end(), val) -
               sorted.begin();
    };

    int left = 0;
    int right = 1;
    Add(index(arr[0]), 1);
    for (int ind = 0; ind < m_elem; ++ind) {
        char where;
        std::cin >> where;
        if (where == 'R') {
            Add(index(arr[right]), 1);
            ++right;
        } else {
            Add(index(arr[left]), -1);
            ++left;
        }
        int answ = Kth(kth);
        if (answ == kN - 1) {
            std::cout << -1 << '\n';
        } else {
            std::cout << sorted[answ] << '\n';
        }
    }
    return 0;
}