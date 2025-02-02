/* #include <iostream>

int main() {
    std::string str;
    std::cin >> str;

    for (char symbol: str) {
        if (symbol == 'M') {
            std::cout << "No" << '\n';
            break;
        }
        if (symbol == 'R') {
            std::cout << "Yes" << '\n';
            break;
        }
    }
    return 0;
} */

/* #include <iostream>

int64_t Choose(int64_t money) {
    int64_t first = 0;
    int64_t second = 0;
    int64_t third = 0;

    int64_t counter = 1;

    if (money < 7) {
        return -1;
    }

    while (money > 0) {
        if ((money & 1) == 1) {
            third = second;
            second = first;
            first = counter;
        }
        money = money >> 1;
        counter = counter << 1;
        //std::cout << money << " "<< counter <<'\n';
    }

    if (third == 0) {
        if (second == 0) {
            first = first >> 1;
            second = first >> 1;
            third = second >> 1;
        } else {
            second = second >> 1;
            third = second >> 1;
        }
    }

    return first + second + third;
}

int main() {
    int number = 0;
    std::cin >> number;

    for (int i = 0; i < number; ++i) {
        int64_t money = 0;
        std::cin >> money;

        std::cout << Choose(money) << '\n';
    }

    return 0;
} */


/* #include <algorithm>
#include <iostream>
#include <vector>

int Correct(std::vector<int>& plan, int number, int left, int right, int good_number) {
    int left_index = 0;
    int right_index = 0;

    for (int i = 0; i < number; ++i) {
        if (plan[i] == left) {
            left_index = i;
            break;
        }
    }

    for (int j = number - 1; j >= 0; --j) {
        if (plan[j] == right) {
            right_index = j;
            break;
        }
    }

    int mid_number = right_index - left_index + 1;

    if (mid_number >= good_number) {
        return 0;
    }

    //std::cout << mid_number <<'\n';

    std::vector<int> left_array = std::vector<int>(good_number - mid_number + 1, 0);
    std::vector<int> right_array = std::vector<int>(good_number - mid_number + 1, 0);

    left_array[good_number - mid_number] = 0;

    if (left_index != 0) {

        int index = left_index - 1;
        for (int i = static_cast<int>(left_array.size()) - 2; i >= 0; --i) {
            if (plan[index] < plan[index + 1]) {
                left_array[i] = left_array[i + 1] + plan[index] - left;
            } else {
                left_array[i] = left_array[i + 1];
            }

            left_array[i] = left - plan[index];

            index --;

            if (index < 0) {
                break;
            }
        }
    }

    right_array[0] = 0;

    if (right_index != number - 1) {

        int index = right_index + 1;
        for (int i = 1; i < static_cast<int>(right_array.size()); ++i) {
            //std::cout << plan[index] << " " << plan[index - 1] <<'\n';
            //if (plan[index] > plan[index - 1]) {
            //    right_array[i] = right_array[i - 1] + plan[index] - right;
            //} else {
            //    right_array[i] = right_array[i - 1];
            //}

            right_array[i] = plan[index] - right;

            index ++;

            if (index == static_cast<int>(plan.size())) {
                break;
            }
        }
    }

    // for (int i = 0; i < static_cast<int>(left_array.size()); ++i) {
    //    std::cout << left_array[i] << " ";
    //}
    //std::cout << '\n';

    //for (int i = 0; i < static_cast<int>(right_array.size()); ++i) {
    //    std::cout << right_array[i] << " ";
    //}
    //std::cout << '\n';

    int minimum = number;
    for (int i = 0; i < static_cast<int>(left_array.size()); ++i) {
        if ((left_array[i] == 0 && i != static_cast<int>(left_array.size()) - 1) || (right_array[i] == 0 && i != 0) ) {
            continue;
        }

        if (left_array[i] + right_array[i] < minimum) {
            minimum = left_array[i] + right_array[i];
        }
    }

    return minimum;
}

int main() {
    int number = 0;
    int good_number = 0;

    std::cin >> number >> good_number;
    
    good_number += 2;

    std::vector<int> plan = std::vector<int>(number);

    for (int i = 0; i < number; ++i) {
        std::cin >> plan[i];
    }

    int left = plan[0];
    int right = plan[1];

    std::sort(plan.begin(), plan.end());

    // for (int i = 0; i < static_cast<int>(plan.size()); ++i) {
    //    std::cout << plan[i] << " ";
    //}
    //std::cout << '\n';

    std::cout << Correct(plan, number, left, right, good_number) <<'\n';

    return 0;
} */

/* #include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

struct Dot {
    int x = 0;
    int y = 0;

    bool operator==(Dot dot) const {
        return this->x == dot.x && this->y == dot.y;
    }
};


int CountLines (int number, std::vector<Dot>& dots) {
    std::map<std::pair<double, double>, std::vector<Dot>> lines;
    const double kEps = 0.000001;

    std::vector<std::pair<double, double>> k_b_coefs;
    for (int i = 0; i < number; ++i) {
        for (int j = i + 1; j < number; ++j) {

            const double kBig = 1000000001;
            double k_coef = kBig;
            double b_coef = kBig;

            if (dots[i].x != dots[j].x) {
                k_coef = (dots[i].y - dots[j].y)/(dots[i].x - dots[j].x);
                b_coef = dots[i].y - k_coef*dots[i].x;
            }

            bool flag = false;
            for (int i = 0; i < static_cast<int>(k_b_coefs.size()); ++i) {
                if (k_coef > k_b_coefs[i].first - kEps && k_coef < k_b_coefs[i].first + kEps && b_coef > k_b_coefs[i].second - kEps  && b_coef < k_b_coefs[i].second + kEps ) {
                    k_coef = k_b_coefs[i].first;
                    b_coef = k_b_coefs[i].second;
                    flag = true;
                }
            }

            if (!flag) {
                k_b_coefs.push_back(std::make_pair(k_coef, b_coef));
            }

            std::pair<double, double> line = std::make_pair(k_coef, b_coef);

            if (lines.find(line) == lines.end()) {
                lines[line].push_back(dots[i]);
                lines[line].push_back(dots[j]);
            } else {
                if (std::find(lines[line].begin(), lines[line].end(), (dots[i])) == lines[line].end()) {
                    lines[line].push_back(dots[i]);
                }
                if (std::find(lines[line].begin(), lines[line].end(), (dots[i])) == lines[line].end()) {
                    lines[line].push_back(dots[j]);
                }
            }
        }
    }

    int counter = 0;
    int empty = 0;

    while (empty != lines.size() - 1) {
        std::pair<double, double> max_line;
        int maximum = 0;

        for (auto& element: lines) {
            if (element.second.size() > maximum) {
                maximum = element.second.size();
                max_line = element.first;
            }
        }

        if (lines[max_line].size() < 2) {
            break;
        }

        Dot first_dot = lines[max_line].back();
        lines[max_line].pop_back();

        Dot second_dot = lines[max_line].back();
        lines[max_line].pop_back();

        if (lines[max_line].empty()) {
            empty += 1;
        }

        for (auto it = lines.begin(); it != lines.end(); ++it) {
            if (it->second.empty()) {
                continue;
            }

            int ii = 0;
            while (ii < static_cast<int>(it->second.size())) {
                if (it->second[ii] == first_dot || it->second[ii] == second_dot) {
                    it->second.erase(it->second.begin() + ii);
                }
                ++ii;
            }

            if (it->second.empty()) {
                empty += 1;
            }
        }

        Dot third_dot;

        for (auto it = lines.begin(); it != lines.end(); ++it) {
            if (it->second.empty()) {
                continue;
            }

            if (it->first != max_line) {
                third_dot = Dot{it->second.back().x, it->second.back().y};
                it->second.pop_back();

                if (it->second.empty()) {
                    empty += 1;
                }

                break;
            }
        }

        for (auto it = lines.begin(); it != lines.end(); ++it) {
            if (it->second.empty()) {
                continue;
            }

            int ii = 0;
            while (ii < static_cast<int>(it->second.size())) {
                if (it->second[ii] == third_dot) {
                    it->second.erase(it->second.begin() + ii);
                }
                ++ii; 
            }

            if (it->second.empty()) {
                empty += 1;
            }
        }

        counter += 1;
    }

    return counter;
}

int main() {
    int number = 0;
    std::cin >> number;

    std::vector<Dot> dots = std::vector<Dot>(number);

    for (int i = 0; i < number; ++i) {
        std::cin >> dots[i].x >> dots[i].y;
    }

    std::cout << CountLines(number, dots) << '\n';
    return 0;
} */


#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

struct Remain {
    int64_t bio = 0;
    int first_pos = 0;
    int second_pos = 0;
    int third_pos = 0;
    int64_t first_remainder = -1;
    int64_t second_remainder = -1;
    int64_t third_remainder = -1;
};

int64_t RemainderDist(Remain& one, Remain& two) {
    return std::min(one.bio - one.second_remainder + two.bio - two.first_remainder, one.bio - one.first_remainder + two.bio - two.second_remainder);
}

int64_t RemainderDist2(Remain& one, Remain& two)

int64_t Correct(int number, int64_t xx, int64_t yy, int64_t zz, std::vector<int64_t>& numbers) {
    Remain x_remain;
    x_remain.bio = xx;
    Remain y_remain;
    y_remain.bio = yy;
    Remain z_remain;
    z_remain.bio = zz;
    Remain xy_remain;
    xy_remain.bio = xx * yy;
    Remain yz_remain;
    yz_remain.bio = yy* zz;
    Remain xz_remain;
    xz_remain.bio = xx * zz;
    Remain xyz_remain;
    xyz_remain.bio = xx * yy * zz;

    for (int i = 0; i < number; ++i) {
        int64_t x_rem = numbers[i] % xx;
        if (x_rem == 0 || x_rem > x_remain.first_remainder) {
            x_remain.third_remainder = x_remain.second_remainder;
            x_remain.third_pos = x_remain.second_pos;
            x_remain.second_remainder = x_remain.first_remainder;
            x_remain.second_pos = x_remain.first_pos;
            if (x_rem == 0) {
                x_remain.first_remainder = xx;
            } else {
                x_remain.first_remainder = x_rem;
            }
            x_remain.first_pos = i;
        }

        int64_t y_rem = numbers[i] % yy;
        if (y_rem == 0 || y_rem > y_remain.first_remainder) {
            y_remain.third_remainder = y_remain.second_remainder;
            y_remain.third_pos = y_remain.second_pos;
            y_remain.second_remainder = y_remain.first_remainder;
            y_remain.second_pos = y_remain.first_pos;
            if (y_rem == 0) {
                y_remain.first_remainder = yy;
            } else {
                y_remain.first_remainder = y_rem;
            }
            y_remain.first_pos = i;
        }

        int64_t z_rem = numbers[i] % zz;
        if (z_rem == 0 || z_rem > z_remain.first_remainder) {
            z_remain.third_remainder = z_remain.second_remainder;
            z_remain.third_pos = z_remain.second_pos;
            z_remain.second_remainder = z_remain.first_remainder;
            z_remain.second_pos = z_remain.first_pos;
            if (z_rem == 0) {
                z_remain.first_remainder = zz;
            } else {
                z_remain.first_remainder = z_rem;
            }
            z_remain.first_pos = i;
        }

        int64_t xy_rem = numbers[i] % xx * numbers[i] % yy;
        if (xy_rem == 0 || xy_rem > xy_remain.first_remainder) {
            xy_remain.third_remainder = xy_remain.second_remainder;
            xy_remain.third_pos = xy_remain.second_pos;
            xy_remain.second_remainder = xy_remain.first_remainder;
            xy_remain.second_pos = xy_remain.first_pos;
            if (xy_rem == 0) {
                xy_remain.first_remainder = xx * yy;
            } else {
                xy_remain.first_remainder = xy_rem;
            }
            xy_remain.first_pos = i;
        }

        int64_t xz_rem = numbers[i] % xx * numbers[i] % zz;
        if (xz_rem == 0 || xz_rem > xz_remain.first_remainder) {
            xz_remain.third_remainder = xz_remain.second_remainder;
            xz_remain.third_pos = xz_remain.second_pos;
            xz_remain.second_remainder = xz_remain.first_remainder;
            xz_remain.second_pos = xz_remain.first_pos;
            if (xz_rem == 0) {
                xz_remain.first_remainder = xx*zz;
            } else {
                xz_remain.first_remainder = xz_rem;
            }
            xz_remain.first_pos = i;
        }

        int64_t yz_rem = numbers[i] % yy * numbers[i] % zz;
        if (yz_rem == 0 || yz_rem > yz_remain.first_remainder) {
            yz_remain.third_remainder = yz_remain.second_remainder;
            yz_remain.third_pos = yz_remain.second_pos;
            yz_remain.second_remainder = yz_remain.first_remainder;
            yz_remain.second_pos = yz_remain.first_pos;
            if (yz_rem == 0) {
                yz_remain.first_remainder = yy * zz;
            } else {
                yz_remain.first_remainder = yz_rem;
            }
            yz_remain.first_pos = i;
        }

        int64_t xyz_rem = numbers[i] % xx * numbers[i] % yy * numbers[i] % zz;
        if (xyz_rem == 0 || xyz_rem > xyz_remain.first_remainder) {
            xyz_remain.third_remainder = xyz_remain.second_remainder;
            xyz_remain.third_pos = xyz_remain.second_pos;
            xyz_remain.second_remainder = xyz_remain.first_remainder;
            xyz_remain.second_pos = xyz_remain.first_pos;
            if (xyz_rem == 0) {
                xyz_remain.first_remainder = xx * yy * zz;
            } else {
                xyz_remain.first_remainder = xyz_rem;
            }
            xyz_remain.first_pos = i;
        }
    }

    std::vector<int64_t> distances;

    if (x_remain.first_pos != y_remain.first_pos && y_remain.first_pos != z_remain.first_pos && x_remain.first_pos != z_remain.first_pos) {
        distances.push_back(x_remain.bio - x_remain.first_remainder + y_remain.bio - y_remain.first_remainder + z_remain.bio - z_remain.first_remainder);
    } else {
        if (x_remain.first_pos == y_remain.first_pos && x_remain.first_pos != y_remain.first_pos) {
            distances.push_back(RemainderDist(x_remain, y_remain));
        } else {
            if (x_remain.first_pos == z_remain.first_pos && x_remain.first_pos != y_remain.first_pos) {
                distances.push_back(RemainderDist(x_remain, z_remain));
            } else {
                if (z_remain.first_pos == y_remain.first_pos && x_remain.first_pos != y_remain.first_pos) {
                    distances.push_back(RemainderDist(x_remain, y_remain));
                } 
                
            }
        }
    }

    if (xy_remain.first_pos != z_remain.first_pos) {
        distances.push_back(xy_remain.bio - xy_remain.first_remainder + z_remain.bio - z_remain.first_remainder);
    } else {
        distances.push_back(RemainderDist(xy_remain, z_remain));
    }

    if (yz_remain.first_pos != x_remain.first_pos) {
        distances.push_back(yz_remain.bio - yz_remain.first_remainder + x_remain.bio - x_remain.first_remainder);
    } else {
        distances.push_back(RemainderDist(yz_remain, x_remain));
    }

    if (xz_remain.first_pos != y_remain.first_pos) {
        distances.push_back(xz_remain.bio - xz_remain.first_remainder + y_remain.bio - y_remain.first_remainder);
    } else {
        distances.push_back(RemainderDist(xz_remain, y_remain));
    }

    distances.push_back(xyz_remain.bio - xyz_remain.first_remainder);

    return *std::min_element(distances.begin(), distances.end());

}

int main() {
    int number = 0;
    std::cin >> number;

    int64_t xx = 0;
    int64_t yy = 0;
    int64_t zz = 0;

    std::cin >> xx >> yy >> zz;

    std::vector<int64_t> numbers = std::vector<int64_t>(number);

    for (int i = 0; i < number; ++i) {
        std::cin >> numbers[i];
    }

    std::cout << Correct(number, xx, yy, zz, numbers) <<'\n';
    
    return 0;
}