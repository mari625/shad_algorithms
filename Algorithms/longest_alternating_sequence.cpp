#include <climits>
#include <exception>
#include <iostream>
#include <string>
#include <vector>


std::vector<int> FindExtremums(std::vector<int>* sequence_ptr, int length) {
    std::vector<int> sequence = *sequence_ptr;
    std::vector<int> extremum_subsequence;

    extremum_subsequence.push_back(0);

    bool rising = true;
    
    if (sequence[0] > sequence[1]) {
        rising = false;
    }

    for (int i = 1; i < length; ++i) {
        if (sequence[i] > sequence[i - 1] && !rising) {
            rising = true;
            extremum_subsequence.push_back(i - 1);
        }
        if (sequence[i - 1] > sequence[i] && rising) {
            rising = false;
            extremum_subsequence.push_back(i - 1);
        }
    }

    extremum_subsequence.push_back(length - 1);

    if (extremum_subsequence.size() > 1 &&
        sequence[extremum_subsequence[0]] ==
            sequence[extremum_subsequence[1]]) {
        extremum_subsequence.erase(extremum_subsequence.begin() + 1);
    }

    if (extremum_subsequence.size() > 1 &&
        sequence[extremum_subsequence[extremum_subsequence.size() - 1]] ==
            sequence[extremum_subsequence[extremum_subsequence.size() - 2]]) {
        extremum_subsequence.erase(extremum_subsequence.end() - 1);
    }

    return extremum_subsequence;
}


int FindNewIndexOfMin (std::vector<int>* sequence_ptr, int number, int next_number, int next_next_number) {
    std::vector<int> sequence = *sequence_ptr;
    int min_index = next_number;

    while (sequence[min_index - 1] < sequence[number]) {
        if (next_next_number > 0 &&
            sequence[min_index - 1] >= sequence[next_next_number]) {
            break;
        }
        min_index -= 1;
    }
    return min_index;
}

int FindNewIndexOfMax (std::vector<int>* sequence_ptr, int number, int next_number, int next_next_number) {
    std::vector<int> sequence = *sequence_ptr;
    int max_index = next_number;
    while (sequence[max_index - 1] > sequence[number]) {
        if (next_next_number > 0 &&
            sequence[max_index - 1] <= sequence[next_next_number]) {
            break;
        }
        max_index -= 1;
    }
return max_index;
}


std::vector<int> MoveSubsequence(std::vector<int>* sequence_ptr, std::vector<int>* extremum_subsequence_ptr, int length) {
    std::vector<int> sequence = *sequence_ptr;
    std::vector<int> extremum_subsequence = *extremum_subsequence_ptr;
    std::vector<int> subsequence;

    subsequence.push_back(sequence[0]);
    if (length == 1) {
        return subsequence;
    }

    for (int i = 0; i < std::ssize(extremum_subsequence) - 1; ++i) {
        int number = extremum_subsequence[i];
        int next_number = extremum_subsequence[i + 1];
        int next_next_number = 0;

        if (i != std::ssize(extremum_subsequence) - 2) {
            next_next_number = extremum_subsequence[i + 2];
        }

        if (next_number > number + 1) {
            if (sequence[next_number] < sequence[number]) {
                int min_index = FindNewIndexOfMin(sequence_ptr, number, next_number, next_next_number);
                subsequence.push_back(sequence[min_index]);
                extremum_subsequence[i + 1] = min_index;
            } else {
                if (sequence[next_number] > sequence[number]) {
                    int max_index = FindNewIndexOfMax(sequence_ptr, number, next_number, next_next_number);
                    subsequence.push_back(sequence[max_index]);
                    extremum_subsequence[i + 1] = max_index;
                }
            }
        } else {
            subsequence.push_back(sequence[next_number]);
        }
    }

    return subsequence;
}


std::vector<int> FindSubsequence(std::vector<int>* sequence_ptr, int length) {
    std::vector<int> extremum_subsequence = FindExtremums(sequence_ptr, length);

    return MoveSubsequence(sequence_ptr, &extremum_subsequence, length);
}

void Check(std::vector<int> sequence, std::vector<int> expected) {
    std::vector<int> answer = FindSubsequence(&sequence, std::size(sequence));

    if (answer.size() != expected.size()) {
        throw std::runtime_error("Answer size is not equal to expected size!");
    }
    for (int i = 0; i < std::ssize(answer); ++i) {
        if (answer[i] != expected[i]) {
            throw std::runtime_error("Answer and expected are different!");
        }
    }
}

void SimpleTest() {
    Check({1, 2}, {1, 2});
    Check({100}, {100});                //NOLINT
    Check({1, 2, 3}, {1, 2});
    Check({4, 3, 2, 1}, {4, 3});
    Check({1, 2, 3, 1}, {1, 2, 1});
    Check({1, 1, 1, 1, 1}, {1});
    Check({2, 2, 1, 1}, {2, 1});
    Check({1, 1}, {1});
    Check({1, 2, 2, 1}, {1, 2, 1});
    Check({1, 1, 2, 2, 1, 1, 2, 2}, {1, 2, 1, 2});
    std::cout << "Simple test passed" << '\n';
}

void LongTest() {
    Check({1, 4, 2, 3, 5, 8, 6, 7, 9, 10}, {1, 4, 2, 8, 6, 7});  //NOLINT
    Check({1, 2, 3, 4, 3, 2, 4, 5}, {1, 3, 2, 4});
    Check({1, 2, 3, 2, 1, 2, 3, 2, 1}, {1, 2, 1, 2, 1});
    Check({1, 2, 4, 8, 7, 6, 5, 6, 9}, {1, 8, 7, 9});   //NOLINT
    Check({3, 17, 17, 3, 17, 158, 17, 7, 20, 7, 27, 27}, //NOLINT
          {3, 17, 3, 17, 7, 20, 7, 27});                    //NOLINT
    Check({100, 100, 200, 200, 100, 100, 200, 200, 515, 515, 200, 200, 100, 300, //NOLINT
           300, 200, 200, 300, 300}, //NOLINT
          {100, 200, 100, 200, 100, 300, 200, 300}); //NOLINT
    std::cout << "Long test passed" << '\n';
}

int main() {
    SimpleTest();
    LongTest();

    int length = 0;
    std::vector<int> sequence;

    std::cin >> length;
    for (int i = 0; i < length; ++i) {
        int element = 0;
        std::cin >> element;
        sequence.push_back(element);
    }

    std::vector<int> subsequence = FindSubsequence(&sequence, length);

    for (int i = 0; i < std::ssize(subsequence); ++i) {
        std::cout << subsequence[i] << " ";
    }
    std::cout << '\n';

    return 0;
}
