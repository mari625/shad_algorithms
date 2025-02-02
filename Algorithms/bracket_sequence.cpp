#include <exception>
#include <iostream>
#include <string>
#include <vector>

std::string CorrectBracketSequence(const std::string& sequence) {
    std::vector<char> bracket_container;
    std::vector<char> opening_brackets = {'(', '{', '['};
    std::vector<char> closing_brackets = {')', '}', ']'};
    int maximum_length = 0;
    bool incorrect_closing_bracket = false;

    for (int i = 0; i < std::ssize(sequence); ++i) {
        bracket_container.push_back(sequence[i]);
        maximum_length = i;
        for (int j = 0; j < std::ssize(closing_brackets); ++j) {
            if (sequence[i] == closing_brackets[j]) {
                if (bracket_container.size() > 1 &&
                    bracket_container[bracket_container.size() - 2] ==
                        opening_brackets[j]) {
                    bracket_container.pop_back();
                    bracket_container.pop_back();
                } else {
                    incorrect_closing_bracket = true;
                    maximum_length -= 1;
                    break;
                }
            }
        }
        if (incorrect_closing_bracket) {
            break;
        }
    }

    if (bracket_container.empty()) {
        return "CORRECT";
    }
    return std::to_string(maximum_length + 1);
}

void Check(const std::string& sequence, const std::string& expected) {
    std::string answer = CorrectBracketSequence(sequence);
    
    if (answer != expected) {
        std::cout << "Got answer " << answer << " but " << expected
                  << "is expected" << '\n';
        throw std::runtime_error("");
    }
}

void SimpleTest() {
    Check("()", "CORRECT");
    Check("(", "1");
    Check("", "CORRECT");
    Check(")", "0");
    Check("({[]})", "CORRECT");
    Check("()()", "CORRECT");
    std::cout << "Simple test passed" << '\n';
}

void IncorrectInputTest() {
    Check("{))))", "1");
    Check(")}]", "0");
    Check("())))", "2");
    Check("()[}]", "3");
    Check("()(){((", "7");
    Check("{(})", "2");
    Check("([({", "4");
    std::cout << "Incorrect Input test passed" << '\n';
}

void LongInputTest() {
    Check("(){})(}{}(}){}()}}{())))()})", "4");
    Check("({{[]}[]})()(()()){}{[()]()}", "CORRECT");
    Check("(){}{[([(()())()]]{})]}{}(())", "17");
    Check("({}{(}))(({)})((){()}}}()(({}}[[){}]]}))", "5");
    std::cout << "Long Input test passed" << '\n';
}

int main() {
    SimpleTest();
    IncorrectInputTest();
    LongInputTest();
    std::string sequence;
    std::cin >> sequence;
    std::cout << CorrectBracketSequence(sequence);
    return 0;
}