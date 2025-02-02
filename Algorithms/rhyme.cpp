#include <iostream>
#include <vector>

struct Node {
    int key = 0;
    int size = 0;
    char exists = 'f';
};

void ConstructTree(int begin, int end, int index, std::vector<Node>& tree) {
    if (begin == end) {
        tree[index] = Node{begin, 1, 't'};
        return;
    }

    if (begin + 1 == end) {
        tree[index] = Node{end, 2, 't'};
        tree[2 * index + 1] = Node{begin, 1, 't'};
        return;
    }

    int key = (end + begin + 1) / 2;
    int size = end - begin + 1;
    tree[index] = Node{key, size, 't'};

    ConstructTree(begin, key - 1, 2 * index + 1, tree);
    ConstructTree(key + 1, end, 2 * index + 2, tree);
}

int DeleteFromTree(std::vector<Node>& tree, int number, int current_index) {
    tree[current_index].size -= 1;

    int left_index = 2 * current_index + 1;
    int right_index = 2 * current_index + 2;

    if (tree[current_index].size == 0 && tree[current_index].exists == 't') {
        tree[current_index].exists = 'f';
        return tree[current_index].key;
    }

    if (tree[left_index].size >= number) {
        return DeleteFromTree(tree, number, left_index);
    }

    if (tree[current_index].exists == 't') {
        if (tree[left_index].size + 1 == number) {
            tree[current_index].exists = 'f';
            return tree[current_index].key;
        }

        number -= 1;
    }

    return DeleteFromTree(tree, number - tree[left_index].size, right_index);
}

std::vector<int> Count(int number, int step) {
    std::vector<int> answer;
    std::vector<Node> tree(number * 3);
    ConstructTree(1, number, 0, tree);

    int current_size = number;
    int current_position = 1;

    while (current_size > 0) {
        current_position = (current_position + step - 2) % current_size + 1;
        answer.push_back(DeleteFromTree(tree, current_position, 0));
        current_size -= 1;
    }

    return answer;
}

int main() {
    int number = 0;
    int step = 0;

    std::cin >> number >> step;

    std::vector<int> answer = Count(number, step);

    for (int i = 0; i < number; ++i) {
        std::cout << answer[i] << " ";
    }
    std::cout << '\n';

    return 0;
}