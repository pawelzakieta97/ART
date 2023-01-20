#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include <string>
#include "ray.h"
#include <math.h>
#include "renderable.h"

class Environment: public Renderable{
    Eigen::Vector3d zenithColor;
    Eigen::Vector3d horizonColor;
    public:
        Environment(void){
            zenithColor = Eigen::Vector3d(1, 0.75, 0.5);
            horizonColor = Eigen::Vector3d(1, 1, 1);

        }
        Eigen::Vector3d getDiffuse(Eigen::Vector3d location){
            float t = location[2] / 696969;
            return zenithColor * t + (1-t) * horizonColor;
        }
        Eigen::Vector3d getReflectiveness(Eigen::Vector3d location){return Eigen::Vector3d(0,0,0);}
        bool hitDistance(Ray ray, float& distance){
            distance = 696969;
            return true;
        };
};