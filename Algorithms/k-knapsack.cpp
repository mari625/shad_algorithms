#include <iostream>
#include <vector>
#include <deque>

struct Group {
    int weight;
    int cost;
    int number;
};

void MaximalCost(std::vector<Group>& groups, int size, std::vector<std::deque<int>>& queues, int& maximal_cost, std::vector<std::vector<int>>& answers) {
    return 
}

int main() {
    int number_groups = 0;
    int size = 0;

    std::cin >> number_groups >> size;
    std::vector<Group> groups(number_groups);

    int sum = 0;
    for (int i = 0; i < number_groups; ++i) {
        std::cin >> groups[i].weight >> groups[i].cost >> groups[i].number;
        sum += groups[i].number;
    }

    std::vector<std::deque<int>> queues(number_groups);
    int maximumal_cost = 0;
    std::vector<std::vector<int>> answers(sum, std::vector<int>(size, 0));

    MaximalCost(groups, size, queues, maximumal_cost);

    std::cout<<maximumal_cost<<'\n';

    return 0;
}