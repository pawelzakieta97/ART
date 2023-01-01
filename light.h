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
        Light(){}
        Light(Eigen::Vector3d position, cv::Vec3d color){
            this->position = position;
            this->color = color;
        }
        virtual float getIntensity(Eigen::Vector3d location){
            return 1;
        }
        Eigen::Vector3d getLightToPoint(Eigen::Vector3d point){
            return point - position;
        }
        Ray getLightRay(Eigen::Vector3d location){
            Eigen::Vector3d l2p = location - position;
            return Ray(position, l2p / l2p.norm(), color * getIntensity(location));
        }

};

class PointLight: public Light{
    public:
        PointLight(Eigen::Vector3d position, cv::Vec3d color){
            this->position = position;
            this->color = color;
        }
        float getIntensity(Eigen::Vector3d location){
            float distance = (this->position - location).norm();
            return 1 / distance / distance;
        }
};

class SunLight: Light{
    public:
        float getIntensity(Eigen::Vector3d location){
            return 1;
        }
        Eigen::Vector3d getLightToPoint(Eigen::Vector3d point){
            return -position;
        } 
        Ray getLightRay(Eigen::Vector3d location){
            Eigen::Vector3d l2p = - position;
            return Ray(position, l2p / l2p.norm(), color * getIntensity(location));
        }   
};