#pragma once
#include <cstdlib>
#include "Eigen/Dense"
#include <math.h>

inline double randomDouble() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

Eigen::Vector3d randomUnitVector(){
    double z = randomDouble() * 2 - 1;
    double r = sqrt(1-z*z);
    double bearing = randomDouble() * 2 * M_PI;
    return Eigen::Vector3d(r*cos(bearing), r*sin(bearing), z);
}