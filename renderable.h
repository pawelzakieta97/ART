#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include "material.h"

class Renderable{
    public:
        Material* material;
        // TODO: ZMIENIC NA hit I ZWROCIC ODBITY PROMIEN!!!
        virtual bool hitDistance(Ray ray, float& distance){return false;}
        Ray scatter(Ray ray, const Eigen::Vector3d& hitPoint){
            Ray scatteredRay  = material->scatter(ray, hitPoint, getNormal(hitPoint));
            return scatteredRay;
        }        void setMaterial(Material* material){
            this->material = material;
        }

        virtual Eigen::Vector3d getNormal(Eigen::Vector3d location){return Eigen::Vector3d(0,0,1);}
        virtual Eigen::Vector3d getDiffuse(Eigen::Vector3d location){return material->diffuse;}
        virtual Eigen::Vector3d getReflectiveness(Eigen::Vector3d location){return material->reflectiveness;}
        virtual float getRoughness(Eigen::Vector3d location){return material->roughness;}
};