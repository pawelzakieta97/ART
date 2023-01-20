#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"


class Ray{
    public:
        Eigen::Vector3d start;
        Eigen::Vector3d direction;
        Eigen::Vector3d color;
        Ray();
        Ray(Eigen::Vector3d start, Eigen::Vector3d direction, Eigen::Vector3d color);
        Eigen::Vector3d getHitLocation(float distance);
};