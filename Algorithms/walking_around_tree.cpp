#include <iostream>
#include <stack>
#include <vector>

std::vector<int> Inorder(std::vector<int>& keys, int number) {
    std::vector<int> inorder_keys;
    std::stack<int> container;

    for (int i = 0; i < number; ++i) {
        if (i != number - 1) {
            if (keys[i + 1] <= keys[i]) {
                container.push(keys[i]);
            } else {
                container.push(keys[i]);
                while (container.top() < keys[i + 1]) {
                    int top = container.top();
                    inorder_keys.push_back(top);
                    container.pop();
                    if (container.empty()) {
                        break;
                    }
                }
            }
        } else {
            container.push(keys[i]);
            while (!container.empty()) {
                int top = container.top();
                inorder_keys.push_back(top);
                container.pop();
            }
        }
    }
    return inorder_keys;
}

std::vector<int> GetLevels(std::vector<int>& keys, int number) {
    std::vector<int> levels(number);
    std::stack<int> keys_stack;
    std::stack<int> levels_stack;

    keys_stack.push(keys[0]);
    levels_stack.push(1);
    levels[0] = 1;

    int add = 0;

    for (int i = 1; i < number; ++i) {
        if (keys[i] < keys[i - 1]) {  // if this less than before put key with
                                      // level in stack
            keys_stack.push(keys[i]);
            levels_stack.push(levels_stack.top() + 1);
            levels[i] = levels_stack.top();

        } else {  // if bigger remove all elements less than it
            int element = levels_stack.top();

            while (keys_stack.top() <= keys[i]) {
                element = levels_stack.top();
                keys_stack.pop();
                levels_stack.pop();
                if (keys_stack.empty()) {
                    break;
                }
            }

            if (levels_stack.empty()) {
                add += 1;
                element = add;
            }

            keys_stack.push(keys[i]);
            levels_stack.push(element + 1);

            levels[i] = levels_stack.top();
        }
    }

    return levels;
}

std::vector<int> Postorder(std::vector<int>& keys, int number) {
    std::vector<int> postorder_keys;
    std::stack<std::vector<int>> container;

    std::vector<int> levels = GetLevels(keys, number);

    container.push({keys[0], levels[0]});

    for (int i = 1; i < number; ++i) {
        if (levels[i] > levels[i - 1]) {
            container.push({keys[i], levels[i]});
        } else {
            if (levels[i] == levels[i - 1]) {
                postorder_keys.push_back(container.top()[0]);
                container.pop();
                container.push({keys[i], levels[i]});
            } else {
                while (container.top()[1] >= levels[i]) {
                    postorder_keys.push_back(container.top()[0]);
                    container.pop();
                    if (container.empty()) {
                        break;
                    }
                }
                container.push({keys[i], levels[i]});
            }
        }
    }

    while (!container.empty()) {
        postorder_keys.push_back(container.top()[0]);
        container.pop();
    }

    return postorder_keys;
}

int main() {
    int number;
    std::cin >> number;

    std::vector<int> keys(number);
    for (int i = 0; i < number; ++i) {
        std::cin >> keys[i];
    }

    std::vector<int> inorder_keys(number);
    inorder_keys = Inorder(keys, number);

    std::vector<int> postorder_keys(number);
    postorder_keys = Postorder(keys, number);

    for (int i = 0; i < number; ++i) {
        std::cout << postorder_keys[i] << " ";
    }
    std::cout << '\n';

    for (int i = 0; i < number; ++i) {
        std::cout << inorder_keys[i] << " ";
    }
    std::cout << '\n';

    return 0;
}