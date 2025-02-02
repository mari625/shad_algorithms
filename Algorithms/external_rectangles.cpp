#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

struct Segment {
    int left;
    int right;

    bool operator<(Segment segment) const { return this->left < segment.left; }

    bool operator==(Segment segment) const {
        return this->left == segment.left;
    }
};

struct Event {
    Segment intersection;
    int height;
    bool begin;
};

bool TimeComparator(Event first, Event second) {
    if (first.height != second.height) {
        return first.height < second.height;
    }
    return first.intersection.left < second.intersection.left;
}

struct Node {
    Segment key;
    int priority;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

std::shared_ptr<Node> Merge(std::shared_ptr<Node> first,
                            std::shared_ptr<Node> second) {
    if (first == nullptr) {
        return second;
    }

    if (second == nullptr) {
        return first;
    }

    if (first->priority < second->priority) {
        first->right = Merge(first->right, second);
        return first;
    }
    second->left = Merge(first, second->left);
    return second;
}

std::vector<std::shared_ptr<Node>> Split(std::shared_ptr<Node> root,
                                         Segment lambda) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    }

    if (root->key == lambda) {
        return {root->left, root->right};
    }

    if (root->key < lambda) {
        return Split(root->right, lambda);
    }
    return Split(root->left, lambda);
}

void Insert(std::shared_ptr<Node> treap, Segment new_segment,
            std::mt19937* generator) {
    const int kBigConstant = 100'000;
    std::vector<std::shared_ptr<Node>> trees = Split(treap, new_segment);
    auto node = std::make_shared<Node>(
        Node{new_segment,
             std::uniform_int_distribution<int>(1, kBigConstant)(*generator),
             nullptr, nullptr});  //
    treap = Merge(Merge(trees[0], node), trees[1]);
}

void Delete(std::shared_ptr<Node> treap, Segment segment) {
    std::vector<std::shared_ptr<Node>> trees = Split(treap, segment);
    treap = Merge(trees[0], trees[1]);
}

bool LookUpToInsert(std::shared_ptr<Node> treap, Segment segment) {
    if (treap == nullptr) {
        return true;
    }

    if (treap->key.left > segment.left) {
        return LookUpToInsert(treap->left, segment);
    }
    if (treap->key.right < segment.left) {
        return LookUpToInsert(treap->right, segment);
    }

    return treap->key.left >= segment.left || treap->key.right <= segment.right;
}

bool LookUpToDelete(std::shared_ptr<Node> treap, Segment segment) {
    if (treap == nullptr) {
        return false;
    }

    if (treap->key.left > segment.left) {
        return LookUpToInsert(treap->left, segment);
    }
    if (treap->key.right < segment.left) {
        return LookUpToInsert(treap->right, segment);
    }

    return treap->key == segment;
}

int CountRectangles(int number, std::vector<Event>& events) {
    int counter = 0;
    std::mt19937 generator;
    const int kSeed = 12;
    generator.seed(kSeed);
    const int kBigConstant = 100'000;

    std::shared_ptr<Node> treap = std::make_shared<Node>(
        Node{events[0].intersection,
             std::uniform_int_distribution<int>(1, kBigConstant)(generator),
             nullptr, nullptr});

    for (int i = 1; i < number; ++i) {
        if (events[i].begin && LookUpToInsert(treap, events[i].intersection)) {
            Insert(treap, events[i].intersection, &generator);
        } else {
            if (LookUpToDelete(treap, events[i].intersection)) {
                Delete(treap, events[i].intersection);
                counter += 1;
            }
        }
    }
    return counter;
}

int main() {
    int number = 0;
    std::cin >> number;

    std::vector<Event> events;

    for (int i = 0; i < number; ++i) {
        int a_side = 0;
        int b_side = 0;
        int c_side = 0;
        int d_side = 0;
        std::cin >> a_side >> b_side >> c_side >> d_side;

        if (a_side > c_side) {
            std::swap(a_side, c_side);
        }

        if (b_side > d_side) {
            std::swap(b_side, d_side);
        }

        events.push_back(Event{Segment{a_side, c_side}, b_side, true});
        events.push_back(Event{Segment{a_side, c_side}, d_side, false});
    }

    std::sort(events.begin(), events.end(), TimeComparator);

    std::cout << CountRectangles(number, events);
    return 0;
}