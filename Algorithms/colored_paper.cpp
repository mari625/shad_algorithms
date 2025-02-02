#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

struct HorizontalLine {
    int number;
    int left;
    int right;
    int height;
    char begin = 't';
};

struct Color {
    int color;
    char deleted = '0';
};

bool LineComparator(HorizontalLine& first, HorizontalLine& second) {
    if (first.height == second.height) {
        return first.number < second.number;
    }
    return first.height < second.height;
}

std::vector<int> CountColors(std::vector<int>& borders,
                             std::vector<HorizontalLine>& lines,
                             std::vector<Color>& colors_by_number) {
    const int kAllColors = 2501;

    std::vector<int> colors(kAllColors, 0);

    for (int i = 1; i < std::ssize(borders); ++i) {
        int left_border = borders[i - 1];
        int right_border = borders[i];

        if (left_border == right_border) {
            continue;
        }

        for (int i = 0; i < std::ssize(colors_by_number); ++i) {
            colors_by_number[i].deleted = '0';
        }

        std::priority_queue<int> heap_of_levels;
        heap_of_levels.push(lines[0].number);
        auto previous_line = lines[0];

        for (int j = 1; j < std::ssize(lines); ++j) {
            auto& current_line = lines[j];

            if (current_line.left > left_border ||
                current_line.right < right_border) {
                continue;
            }

            int previous_level = heap_of_levels.top();

            heap_of_levels.push(current_line.number);

            int begin = previous_line.height;
            int end = current_line.height;

            int color = colors_by_number[previous_level].color;
            colors[color] += (end - begin) * (right_border - left_border);

            if (current_line.begin == 'f') {
                colors_by_number[current_line.number].deleted = '1';
            }

            while (!heap_of_levels.empty() &&
                   colors_by_number[heap_of_levels.top()].deleted == '1') {
                heap_of_levels.pop();
            }

            previous_line = current_line;
        }
    }

    return colors;
}

int main() {
    int width = 0;
    int length = 0;
    int number = 0;
    std::cin >> width >> length >> number;

    std::vector<HorizontalLine> lines((number + 1) * 2);
    std::vector<int> borders((number + 1) * 2);
    std::vector<Color> colors_by_number(number + 1);

    lines[0] = HorizontalLine{0, 0, width, 0};
    lines[number + 1] = HorizontalLine{0, 0, width, length, 'f'};

    borders[0] = 0;
    borders[number + 1] = width;

    colors_by_number[0] = Color{1};

    for (int i = 1; i < number + 1; ++i) {
        int left_x = 0;
        int left_y = 0;
        int right_x = 0;
        int right_y = 0;
        int color = 0;

        std::cin >> left_x >> left_y >> right_x >> right_y >> color;
        borders[i] = left_x;
        borders[i + number + 1] = right_x;
        lines[i] = HorizontalLine{i, left_x, right_x, left_y};
        lines[i + number + 1] =
            HorizontalLine{i, left_x, right_x, right_y, 'f'};
        colors_by_number[i] = Color{color};
    }

    std::sort(lines.begin(), lines.end(), LineComparator);
    std::sort(borders.begin(), borders.end());

    std::vector<int> answer = CountColors(borders, lines, colors_by_number);

    for (int i = 0; i < std::ssize(answer); ++i) {
        if (answer[i] != 0) {
            std::cout << i << " " << answer[i] << '\n';
        }
    }
    return 0;
}