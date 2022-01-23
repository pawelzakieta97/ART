#pragma once

#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include <string>
#include "ray.h"
#include <math.h>
#include "renderable.h"
#include "camera.h"
#include <memory>

class Light{
    public:
        Eigen::Vector3d position;
        cv::Vec3d color;
        Light(Eigen::Vector3d position, cv::Vec3d color){
            this->position = position;
            this->color = color;
        }
        float getIntensity(Eigen::Vector3d location){
            return 1;
        }

};

class PointLight: Light{
    public:
        float getIntensity(Eigen::Vector3d location){
            float distance = (this->position - location).norm();
            return 1 / distance / distance;
        }
};