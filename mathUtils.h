#pragma once
#ifndef MATHSUTILS_H
#define MATHSUTILS_H
#include <random>

// usings.
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// constants.
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// convert degrees to radians.
inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

// returns a random real in [0,1].
inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// returns a random real in [min,max].
inline double randomDdouble(double min, double max) {
    return min + (max - min) * randomDouble();
}

// clamp the value x to the range [min,max].
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// common headers.
#include "ray.h"
#include "vec3.h"

#endif
