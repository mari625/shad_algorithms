#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

struct Intersection {
    double left_border = std::nan("");
    double right_border = std::nan("");
};

struct Intersection FindIntersection(std::vector<int>* center_ptr,
                                     double radius) {
    std::vector<int> center = *center_ptr;
    double center_x = static_cast<double>(center[0]);
    double center_y = static_cast<double>(center[1]);
    struct Intersection intersection;

    if (fabs(radius) > fabs(center_y)) {
        intersection.left_border =
            center_x - sqrt(pow(radius, 2) - pow(center_y, 2));
        intersection.right_border =
            center_x + sqrt(pow(radius, 2) - pow(center_y, 2));
    }

    return intersection;
}

std::vector<Intersection> FindAllIntersections(
    std::vector<std::vector<int>>* dots_ptr, int number_dots, double radius) {
    std::vector<std::vector<int>> dots = *dots_ptr;
    std::vector<Intersection> intersections;

    for (int i = 0; i < number_dots; ++i) {
        std::vector<int> center = dots[i];
        struct Intersection intersection = FindIntersection(&center, radius);
        if (!std::isnan(intersection.left_border)) {
            intersections.push_back(intersection);
        }
    }

    return intersections;
}

struct Bracket {
    double dot;
    int value;
};

bool BracketComparator(Bracket first, Bracket second) {
    return first.dot < second.dot;
}

struct Sector {
    double left_border = 0;
    double right_border = 0;
    double distance = 0;
    int number_of_circles = 0;
};

bool SectorComparator(Sector first, Sector second) {
    return first.number_of_circles < second.number_of_circles;
}

std::vector<Sector> FindAllAreas(std::vector<Intersection>* intersections_ptr) {
    std::vector<Intersection> intersections = *intersections_ptr;
    std::vector<Bracket> dots;
    for (int i = 0; i < std::ssize(intersections); ++i) {
        dots.push_back(Bracket{intersections[i].left_border, 1});
        dots.push_back(Bracket{intersections[i].right_border, -1});
    }

    std::sort(dots.begin(), dots.end(), BracketComparator);

    std::vector<Sector> sectors;
    int current_number_circles = 0;

    for (int i = 0; i < std::ssize(dots) - 1; ++i) {
        if (dots[i].value == 1) {
            current_number_circles += 1;
        } else {
            current_number_circles -= 1;
        }

        sectors.push_back(Sector{dots[i].dot, dots[i + 1].dot,
                                 dots[i + 1].dot - dots[i].dot,
                                 current_number_circles});
    }

    return sectors;
}

double SelectMinimalRadius(std::vector<std::vector<int>>* dots_ptr,
                           int number_dots, int number_selected_dots) {
    const double kBigRadiusConstant = 1000000;

    std::vector<std::vector<int>> dots = *dots_ptr;
    double max_radius = kBigRadiusConstant;
    double min_radius = 0;
    double current_radius = 0;
    const double kEps = 0.001;

    while (min_radius < max_radius - kEps) {
        current_radius = (min_radius + max_radius) / 2;
        std::vector<Intersection> intersections =
            FindAllIntersections(dots_ptr, number_dots, current_radius);
        std::vector<Sector> sectors = FindAllAreas(&intersections);
        Sector max_circle;

        if (!sectors.empty()) {
            max_circle = (*std::max_element(sectors.begin(), sectors.end(),
                                            SectorComparator));
        }

        if (sectors.empty() ||
            max_circle.number_of_circles < number_selected_dots) {
            min_radius = current_radius;
        } else {
            max_radius = current_radius;
        }
    }

    return current_radius;
}

int main() {
    int number_dots = 0;
    int number_choosen_dots = 0;
    std::vector<std::vector<int>> dots;

    std::cin >> number_dots >> number_choosen_dots;
    for (int i = 0; i < number_dots; ++i) {
        std::vector<int> coordinates;
        int x = 0;
        int y = 0;
        std::cin >> x >> y;
        coordinates.push_back(x);
        coordinates.push_back(y);
        dots.push_back(coordinates);
    }

    const int kNumberOfDigits = 7;

    double answer =
        SelectMinimalRadius(&dots, number_dots, number_choosen_dots);
    std::cout << std::setprecision(kNumberOfDigits) << answer << '\n';
    return 0;
}