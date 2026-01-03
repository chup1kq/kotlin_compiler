#include <cmath>
#include <limits>

bool doubleEquals(double a, double b) {
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}