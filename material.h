#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"

class Material{
    public:
        cv::Vec3d diffuse;
        cv::Vec3d reflectiveness;
        float roughness;
        Material(cv::Vec3d diffuse, cv::Vec3d reflectiveness, float roughness){
            this->roughness = roughness;
            this->reflectiveness = reflectiveness;
            this->diffuse = diffuse;
        };
        Material(){
            diffuse.val[0] = 0;
            diffuse.val[1] = 0;
            diffuse.val[1] = 0;
            roughness = 0.1;
            reflectiveness.val[0] = 0;
            reflectiveness.val[1] = 0;
            reflectiveness.val[1] = 0;
        }
};