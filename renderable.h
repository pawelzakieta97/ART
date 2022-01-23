#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include "material.h"

class Renderable{
    public:
        Material material;
        // TODO: ZMIENIC NA hit I ZWROCIC ODBITY PROMIEN!!!
        virtual bool hitDistance(Ray ray, float& distance){return false;};
        void setMaterial(Material material){
            this->material = material;
        }

        virtual Eigen::Vector3d getNormal(Eigen::Vector3d location){return Eigen::Vector3d(0,0,1);}
        virtual cv::Vec3d getDiffuse(Eigen::Vector3d location){return material.diffuse;}
        virtual cv::Vec3d getReflectiveness(Eigen::Vector3d location){return material.reflectiveness;}
        virtual float getRoughness(Eigen::Vector3d location){return material.roughness;}
};