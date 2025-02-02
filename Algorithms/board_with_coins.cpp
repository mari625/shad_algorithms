#include <cmath>
#include <iostream>
#include <queue>
#include <vector>

struct Vertex {
    int board;
    int number;
};

int Solution(int first, int string_number, int column_number) {
    int answer = 0;
    for (int i = 0; i < string_number; ++i) {
        for (int j = 0; j < column_number; ++j) {
            int element = 1 - (((i + j) & 1) ^ first);
            answer = (answer << 1) + element;
        }
    }
    return answer;
}

int GetStringBoard(int i_index, int j_index, int column_number,
                   Vertex current_vertex) {
    if (j_index > 1) {
        int identifier_string = 3 << (i_index * column_number - j_index);
        return current_vertex.board ^ identifier_string;
    }
    return 0;
}

int GetColumnBoard(int i_index, int j_index, int column_number,
                   int string_number, Vertex current_vertex) {
    if (i_index < string_number) {
        int identifier_column =
            ((1 << column_number) + 1)
            << ((string_number - i_index - 1) * column_number + column_number -
                j_index);
        return current_vertex.board ^ identifier_column;
    }
    return 0;
}

void LayerPush(std::vector<char>& visited, std::queue<Vertex>& layer,
               Vertex current_vertex, int new_board) {
    if (visited[new_board] == '0' && new_board != 0) {
        layer.push({new_board, current_vertex.number + 1});
        visited[new_board] = '1';
    }
}

int Solve(int string_number, int column_number, int board) {
    const int kMagic = 21;
    std::vector<char> visited(std::pow(2, kMagic), '0');
    std::queue<Vertex> layer;

    int zero_answer = Solution(0, string_number, column_number);
    int one_answer = Solution(1, string_number, column_number);

    if (board == zero_answer || board == one_answer) {
        return 0;
    }

    layer.push(Vertex{board, 0});
    visited[board] = '1';

    while (!layer.empty()) {
        Vertex current_vertex = layer.front();

        for (int i = 1; i <= string_number; ++i) {
            for (int j = 1; j <= column_number; ++j) {
                int new_board =
                    GetStringBoard(i, j, column_number, current_vertex);

                if (new_board == zero_answer || new_board == one_answer) {
                    return current_vertex.number + 1;
                }

                LayerPush(visited, layer, current_vertex, new_board);

                new_board = GetColumnBoard(i, j, column_number, string_number,
                                           current_vertex);

                if (new_board == zero_answer || new_board == one_answer) {
                    return current_vertex.number + 1;
                }

                LayerPush(visited, layer, current_vertex, new_board);
            }
        }

        visited[current_vertex.board] = '2';
        layer.pop();
    }
    return -1;
}

int main() {
    int number_of_strings = 0;
    int number_of_columns = 0;

    std::cin >> number_of_strings >> number_of_columns;

    std::string board_string{};

    for (int i = 0; i < number_of_strings; ++i) {
        std::string row;
        std::cin >> row;
        board_string += row;
    }

    int board = std::stoi(board_string, 0, 2);

    std::cout << Solve(number_of_strings, number_of_columns, board) << '\n';
    return 0;
}