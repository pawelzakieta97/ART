#pragma once
#include <opencv2/highgui.hpp>
#include "Eigen/Dense"

Eigen::Vector3d multiplyColor(Eigen::Vector3d c1, Eigen::Vector3d c2){
    return Eigen::Vector3d(c1[0]*c2[0], c1[1]*c2[1], c1[2]*c2[2]);
}