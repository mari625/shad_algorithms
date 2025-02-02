#include <iostream>
#include <queue>
#include <vector>

struct Point {
    int x = 0;
    int y = 0;

    bool operator==(Point point) const { return x == point.x && y == point.y; }

    bool operator!=(Point point) const { return x != point.x || y != point.y; }
};

struct SmallPoint {
    Point point;
    char limit = 'n';
    int number = 0;
};

struct BigPoint {
    Point current;
    SmallPoint previous;
    char limit = 'n';
    int number = 0;
};

struct Previous {
    SmallPoint left_limited;
    SmallPoint right_limited;
};

int FirstStep(Point begin, Point end, std::vector<std::vector<char>>& field,
              std::queue<BigPoint>& neighbours,
              std::vector<std::vector<std::vector<Previous>>>& visited) {
    int string_number = field[0].size();
    int column_number = field.size();

    if (begin == end) {
        return 0;
    }

    Point new_point;

    if (begin.x > 1 && field[begin.x - 2][begin.y - 1] == '.') {  // left
        new_point = Point{begin.x - 1, begin.y};
        if (new_point == end) {
            return 1;
        }
        neighbours.push(BigPoint{new_point, SmallPoint{begin}});
    }

    if (begin.x < column_number &&
        field[begin.x][begin.y - 1] == '.') {  // right
        new_point = Point{begin.x + 1, begin.y};
        if (new_point == end) {
            return 1;
        }
        neighbours.push(BigPoint{new_point, SmallPoint{begin}});
    }

    if (begin.y < string_number &&
        field[begin.x - 1][begin.y] == '.') {  // down
        new_point = Point{begin.x, begin.y + 1};
        if (new_point == end) {
            return 1;
        }
        neighbours.push(BigPoint{new_point, SmallPoint{begin}});
    }

    if (begin.y > 1 && field[begin.x - 1][begin.y - 2] == '.') {  // up
        new_point = Point{begin.x, begin.y - 1};
        if (new_point == end) {
            return 1;
        }
        neighbours.push(BigPoint{new_point, SmallPoint{begin}});
    }

    for (int i = 0; i < 4; ++i) {
        visited[begin.x - 1][begin.y - 1][i] =
            Previous{SmallPoint{begin}, SmallPoint{begin}};
    }

    return -1;
}

int LeftToRight(BigPoint& current_place, std::queue<BigPoint>& neighbours,
                std::vector<std::vector<char>>& field,
                std::vector<std::vector<std::vector<Previous>>>& visited) {
    int string_number = field[0].size();
    int column_number = field.size();

    if (current_place.current.x - current_place.previous.point.x > 0) {
        std::cout<<"left"<<'\n';
        if (current_place.current.y > 1 &&
            field[current_place.current.x - 1][current_place.current.y - 2] ==
                '.' &&
            current_place.limit != 'l' &&
            visited[current_place.current.x - 1][current_place.current.y - 2][2]
                    .left_limited.point.x == 0) {
            neighbours.push(BigPoint{
                Point{current_place.current.x, current_place.current.y - 1},
                SmallPoint{current_place.current, current_place.limit, 0}, 'l',
                2});

            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        std::cout<<"right"<<'\n';
        if (current_place.current.y < string_number &&
            field[current_place.current.x - 1][current_place.current.y] ==
                '.' &&
            current_place.limit != 'r' &&
            visited[current_place.current.x - 1][current_place.current.y][3]
                    .right_limited.point.x == 0) {
            neighbours.push(BigPoint{
                Point{current_place.current.x, current_place.current.y + 1},
                SmallPoint{current_place.current, current_place.limit, 0}, 'r',
                3});

            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        std::cout<<"straight"<<'\n';
        if (current_place.current.x < column_number &&
            field[current_place.current.x][current_place.current.y - 1] ==
                '.') {
            if (((current_place.limit == 'l' || current_place.limit == 'n') &&
                 visited[current_place.current.x][current_place.current.y - 1]
                        [0]
                            .left_limited.point.x == 0) ||
                (current_place.limit == 'r' &&
                 visited[current_place.current.x][current_place.current.y - 1]
                        [0]
                            .right_limited.point.x == 0)) {
                neighbours.push(BigPoint{
                    Point{current_place.current.x + 1, current_place.current.y},
                    SmallPoint{current_place.current, current_place.limit, 0},
                    current_place.limit, 0});

                std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
                std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
                std::cout << neighbours.size()<<'\n';
            }
        }
        std::cout<<"back"<<'\n';
        if (((current_place.limit == 'l' || current_place.limit == 'n') &&
             visited[current_place.current.x - 2][current_place.current.y - 1]
                    [1]
                        .left_limited.point.x == 0) ||
            (current_place.limit == 'r' &&
             visited[current_place.current.x - 2][current_place.current.y - 1]
                    [1]
                        .right_limited.point.x == 0)) {
            neighbours.push(BigPoint{
                Point{current_place.current.x - 1, current_place.current.y},
                SmallPoint{current_place.current, current_place.limit, 0},
                current_place.limit, 1});

            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        return 0;
    }
    return -1;
}

int RightToLeft(BigPoint& current_place, std::queue<BigPoint>& neighbours,
                std::vector<std::vector<char>>& field,
                std::vector<std::vector<std::vector<Previous>>>& visited) {
    int string_number = field[0].size();

    if (current_place.current.x - current_place.previous.point.x < 0) {
        std::cout<<"left"<<'\n';
        if (current_place.current.y < string_number &&
            field[current_place.current.x - 1][current_place.current.y] ==
                '.' &&
            current_place.limit != 'l' &&
            visited[current_place.current.x - 1][current_place.current.y][3]
                    .left_limited.point.x == 0) {
            neighbours.push(BigPoint{
                Point{current_place.current.x, current_place.current.y + 1},
                SmallPoint{current_place.current, current_place.limit, 1}, 'l',
                3});

            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        std::cout<<"right"<<'\n';
        if (current_place.current.y > 1 &&
            field[current_place.current.x - 1][current_place.current.y - 2] ==
                '.' &&
            current_place.limit != 'r' &&
            visited[current_place.current.x - 1][current_place.current.y - 2][2]
                    .right_limited.point.x == 0) {
            neighbours.push(BigPoint{
                Point{current_place.current.x, current_place.current.y - 1},
                SmallPoint{current_place.current, current_place.limit, 1}, 'r',
                2});

            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        std::cout<<"straight"<<'\n';
        if (current_place.current.x > 1 &&
            field[current_place.current.x - 2][current_place.current.y - 1] ==
                '.') {
            if (((current_place.limit == 'l' || current_place.limit == 'n') &&
                 visited[current_place.current.x - 2]
                        [current_place.current.y - 1][1]
                            .left_limited.point.x == 0) ||
                (current_place.limit == 'r' &&
                 visited[current_place.current.x - 2]
                        [current_place.current.y - 1][1]
                            .right_limited.point.x == 0)) {
                neighbours.push(BigPoint{
                    Point{current_place.current.x - 1, current_place.current.y},
                    SmallPoint{current_place.current, current_place.limit, 1},
                    current_place.limit, 1});
                
                std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
                std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
                std::cout << neighbours.size()<<'\n';
            }
        }
        std::cout<<"back"<<'\n';
        if (((current_place.limit == 'l' || current_place.limit == 'n') &&
             visited[current_place.current.x][current_place.current.y - 1][0]
                     .left_limited.point.x == 0) ||
            (current_place.limit == 'r' &&
             visited[current_place.current.x][current_place.current.y - 1][0]
                     .right_limited.point.x == 0)) {
            neighbours.push(BigPoint{
                Point{current_place.current.x + 1, current_place.current.y},
                SmallPoint{current_place.current, current_place.limit, 1},
                current_place.limit, 0});
            
            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        return 0;
    }
    return -1;
}

int DownToUp(BigPoint& current_place, std::queue<BigPoint>& neighbours,
             std::vector<std::vector<char>>& field,
             std::vector<std::vector<std::vector<Previous>>>& visited) {
    int column_number = field.size();

    if (current_place.current.y - current_place.previous.point.y < 0) {
        std::cout<<"left"<<'\n';
        if (current_place.current.x > 1 &&
            field[current_place.current.x - 2][current_place.current.y - 1] ==
                '.' &&
            current_place.limit != 'l' &&
            visited[current_place.current.x - 2][current_place.current.y - 1][1]
                    .left_limited.point.x == 0) {
            neighbours.push(BigPoint{
                Point{current_place.current.x - 1, current_place.current.y},
                SmallPoint{current_place.current, current_place.limit, 2}, 'l',
                1});
            
            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        std::cout<<"right"<<'\n';
        if (current_place.current.x < column_number &&
            field[current_place.current.x][current_place.current.y - 1] ==
                '.' &&
            current_place.limit != 'r' &&
            visited[current_place.current.x][current_place.current.y - 1][0]
                    .right_limited.point.x == 0) {
            neighbours.push(BigPoint{
                Point{current_place.current.x + 1, current_place.current.y},
                SmallPoint{current_place.current, current_place.limit, 2}, 'r',
                0});

            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        std::cout<<"straight"<<'\n';
        if (current_place.current.y > 1 &&
            field[current_place.current.x - 1][current_place.current.y - 2] ==
                '.') {
            if (((current_place.limit == 'l' || current_place.limit == 'n') &&
                 visited[current_place.current.x - 1]
                        [current_place.current.y - 2][2]
                            .left_limited.point.x == 0) ||
                (current_place.limit == 'r' &&
                 visited[current_place.current.x - 1]
                        [current_place.current.y - 2][2]
                            .right_limited.point.x == 0)) {
                neighbours.push(BigPoint{
                    Point{current_place.current.x, current_place.current.y - 1},
                    SmallPoint{current_place.current, current_place.limit, 2},
                    current_place.limit, 2});

                std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
                std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
                std::cout << neighbours.size()<<'\n';
            }
        }
        std::cout<<"back"<<'\n';
        if (((current_place.limit == 'l' || current_place.limit == 'n') &&
             visited[current_place.current.x - 1][current_place.current.y][3]
                     .left_limited.point.x == 0) ||
            (current_place.limit == 'r' &&
             visited[current_place.current.x - 1][current_place.current.y][3]
                     .right_limited.point.x == 0)) {
            neighbours.push(BigPoint{
                Point{current_place.current.x, current_place.current.y + 1},
                SmallPoint{current_place.current, current_place.limit, 2},
                current_place.limit, 3});
            
            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        return 0;
    }
    return -1;
}

int UpToDown(BigPoint& current_place, std::queue<BigPoint>& neighbours,
             std::vector<std::vector<char>>& field,
             std::vector<std::vector<std::vector<Previous>>>& visited) {
    int string_number = field[0].size();
    int column_number = field.size();

    if (current_place.current.y - current_place.previous.point.y > 0) {
        std::cout<<"left"<<'\n';
        if (current_place.current.x < column_number &&
            field[current_place.current.x][current_place.current.y - 1] ==
                '.' &&
            current_place.limit != 'l' &&
            visited[current_place.current.x][current_place.current.y - 1][0]
                    .left_limited.point.x == 0) {
            neighbours.push(BigPoint{
                Point{current_place.current.x + 1, current_place.current.y},
                SmallPoint{current_place.current, current_place.limit, 3}, 'l',
                0});

            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        std::cout<<"right"<<'\n';
        if (current_place.current.x > 1 &&
            field[current_place.current.x - 2][current_place.current.y - 1] ==
                '.' &&
            current_place.limit != 'r' &&
            visited[current_place.current.x - 2][current_place.current.y - 1][1]
                    .right_limited.point.x == 0) {
            neighbours.push(BigPoint{
                Point{current_place.current.x - 1, current_place.current.y},
                SmallPoint{current_place.current, current_place.limit, 3}, 'r',
                1});

            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        std::cout<<"straight"<<'\n';
        if (current_place.current.y < string_number &&
            field[current_place.current.x - 1][current_place.current.y] ==
                '.') {
            if (((current_place.limit == 'l' || current_place.limit == 'n') &&
                 visited[current_place.current.x - 1][current_place.current.y]
                        [3]
                            .left_limited.point.x == 0) ||
                ((current_place.limit == 'r' || current_place.limit == 'n') &&
                 visited[current_place.current.x - 1][current_place.current.y]
                        [3]
                            .right_limited.point.x == 0)) {
                neighbours.push(BigPoint{
                    Point{current_place.current.x, current_place.current.y + 1},
                    SmallPoint{current_place.current, current_place.limit, 3},
                    current_place.limit, 3});

                std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
                std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
                std::cout << neighbours.size()<<'\n';
            }
        }
        std::cout<<"back"<<'\n';
        if (((current_place.limit == 'l' || current_place.limit == 'n') &&
             visited[current_place.current.x - 1][current_place.current.y - 2]
                    [2]
                        .left_limited.point.x == 0) ||
            ((current_place.limit == 'r' || current_place.limit == 'n') &&
             visited[current_place.current.x - 1][current_place.current.y - 2]
                    [2]
                        .right_limited.point.x == 0)) {
            neighbours.push(BigPoint{
                Point{current_place.current.x, current_place.current.y - 1},
                SmallPoint{current_place.current, current_place.limit, 3},
                current_place.limit, 2});

            std::cout << neighbours.back().current.x << " " << neighbours.back().current.y<< " " << neighbours.back().limit <<'\n';
            std::cout << neighbours.back().previous.point.x << " " << neighbours.back().previous.point.y << " " << neighbours.back().previous.limit <<'\n';
            std::cout << neighbours.size()<<'\n';
        }
        return 0;
    }
    return -1;
}

void ReconstructWay(std::vector<Point>& way,
                    std::vector<std::vector<std::vector<Previous>>>& visited,
                    Point begin, Point end) {
    SmallPoint current_point;
    for (int i = 0; i < 4; ++i) {
        if (visited[end.x - 1][end.y - 1][i].left_limited.point.x != 0) {
            current_point = visited[end.x - 1][end.y - 1][i].left_limited;
        } else if (visited[end.x - 1][end.y - 1][i].right_limited.point.x !=
                   0) {
            current_point = visited[end.x - 1][end.y - 1][i].right_limited;
        }
    }

    if (current_point.point.x != 0) {
        way.push_back(end);
        way.push_back(current_point.point);
        while (current_point.point != begin) {
            if (current_point.limit == 'l') {
                current_point =
                    visited[current_point.point.x - 1]
                           [current_point.point.y - 1][current_point.number]
                               .left_limited;
            } else {
                current_point =
                    visited[current_point.point.x - 1]
                           [current_point.point.y - 1][current_point.number]
                               .right_limited;
            }
            way.push_back(current_point.point);
        }
    }
}

void AddToVisited(BigPoint& current_place,
                  std::vector<std::vector<std::vector<Previous>>>& visited,
                  int number) {
    if (current_place.limit == 'l') {
        visited[current_place.current.x - 1][current_place.current.y - 1]
               [number]
                   .left_limited = current_place.previous;
    } else if (current_place.limit == 'r') {
        visited[current_place.current.x - 1][current_place.current.y - 1]
               [number]
                   .right_limited = current_place.previous;
    } else {
        visited[current_place.current.x - 1][current_place.current.y - 1]
               [number]
                   .left_limited = current_place.previous;
        visited[current_place.current.x - 1][current_place.current.y - 1]
               [number]
                   .right_limited = current_place.previous;
    }

    std::cout << current_place.current.x <<" "<<current_place.current.y<<'\n';
    std::cout << visited[current_place.current.x - 1][current_place.current.y - 1][number].left_limited.point.x << " " << visited[current_place.current.x - 1][current_place.current.y - 1][number].left_limited.point.y << " " << visited[current_place.current.x - 1][current_place.current.y - 1][number].left_limited.limit<<" " << visited[current_place.current.x - 1][current_place.current.y - 1][number].left_limited.number <<'\n';
    std::cout << visited[current_place.current.x - 1][current_place.current.y - 1][number].right_limited.point.x << " " << visited[current_place.current.x - 1][current_place.current.y - 1][number].right_limited.point.y << " " << visited[current_place.current.x - 1][current_place.current.y - 1][number].right_limited.limit <<" "<< visited[current_place.current.x - 1][current_place.current.y - 1][number].left_limited.number<<'\n';
    std::cout << '\n';
}

std::vector<Point> FindWay(Point begin, Point end,
                           std::vector<std::vector<char>>& field,
                           int string_number, int column_number) {
    std::queue<BigPoint> neighbours;
    std::vector<std::vector<std::vector<Previous>>> visited(
        column_number, std::vector<std::vector<Previous>>(
                           string_number, std::vector<Previous>(4)));
    std::vector<Point> way;

    int fast = FirstStep(begin, end, field, neighbours, visited);
    if (fast == 0) {
        return {begin};
    }

    if (fast == 1) {
        return {end, begin};
    }

    while (!neighbours.empty()) {
        BigPoint current_place = neighbours.front();
        neighbours.pop();
        std::cout << "Top now" <<'\n';
        std::cout << current_place.current.x << " " << current_place.current.y<< " " << current_place.limit <<'\n';
        std::cout << current_place.previous.point.x << " " << current_place.previous.point.y << " " << current_place.previous.limit <<'\n';
        std::cout << neighbours.size()<<'\n';

        std::cout << "Left added" <<'\n';
        int right = LeftToRight(current_place, neighbours, field, visited);

        if (right != -1) {
            AddToVisited(current_place, visited, 0);
        }

        std::cout << "Right added" <<'\n';
        int left = RightToLeft(current_place, neighbours, field, visited);

        if (left != -1) {
            AddToVisited(current_place, visited, 1);
        }

        std::cout << "Down added" <<'\n';
        int up = DownToUp(current_place, neighbours, field, visited);

        if (up != -1) {
            AddToVisited(current_place, visited, 2);
        }

        std::cout << "Up added" <<'\n';
        int down = UpToDown(current_place, neighbours, field, visited);

        if (down != -1) {
            AddToVisited(current_place, visited, 3);
        }

        std::cout<<current_place.current.x<<" "<<current_place.current.y<<" if end"<<'\n';
        if (current_place.current == end) {
            break;
        }
    }

    /* for (int i = 0; i < column_number; ++i) {
        for (int j = 0; j < string_number; ++j) {
            std::cout<<i<<" "<<j<<'\n';
            for (int k = 0; k < 4; ++k) {
                std::cout << visited[i][j][k].left_limited.point.x << " " << visited[i][j][k].left_limited.point.y << " " << visited[i][j][k].left_limited.limit<< " "<<visited[i][j][k].left_limited.number <<'\n';
                std::cout << visited[i][j][k].right_limited.point.x << " " << visited[i][j][k].right_limited.point.y << " " << visited[i][j][k].right_limited.limit <<" "<<visited[i][j][k].left_limited.number<<'\n';
                std::cout << '\n';
            }
            std::cout << '\n';
        }
    } */

    ReconstructWay(way, visited, begin, end);

    return way;
}

int main() {
    int string_number = 0;
    int column_number = 0;

    std::cin >> string_number >> column_number;

    std::vector<std::vector<char>> field(column_number,
                                         std::vector<char>(string_number));

    for (int i = 0; i < string_number; ++i) {
        for (int j = 0; j < column_number; ++j) {
            std::cin >> field[j][i];
        }
    }

    Point begin;
    std::cin >> begin.y >> begin.x;

    Point end;
    std::cin >> end.y >> end.x;

    std::vector<Point> way =
        FindWay(begin, end, field, string_number, column_number);

    if (way.empty()) {
        std::cout << "-1" << '\n';
    } else {
        std::cout << way.size() - 1 << '\n';
        for (int i = std::ssize(way) - 1; i >= 0; --i) {
            std::cout << way[i].y << " " << way[i].x << '\n';
        }
    }

    return 0;
}