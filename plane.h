#pragma once
#include <opencv2/highgui.hpp>
#include <math.h>
#include <iostream>
#include "Eigen/Dense"
#include "renderable.h"
#include "material.h"
#include "ray.h"

class Plane: public Renderable{
    public:
        Eigen::Vector3d normal;
        Eigen::Vector3d origin;
        float height;
        Plane(Eigen::Vector3d normal, Eigen::Vector3d origin){
            this->normal = normal;
            this->origin = origin;
            this->height = normal.dot(origin);
        }
        bool hitDistance(Ray ray, float& distance){
            if (ray.direction.dot(normal) == 0) return false;
            distance = (height - normal.dot(ray.start))/normal.dot(ray.direction);
            if (distance < 0.001) return false;
            return true;
        }
        Eigen::Vector3d getNormal(Eigen::Vector3d location){
            return normal;
        }
        // Ray scatter(Ray ray, const Eigen::Vector3d& hitPoint){
        //     cv::Vec3d scatterTint;
        //     Ray scatteredRay;
        //     scatteredRay.direction = material->scatter(ray, hitPoint, getNormal(hitPoint), scatterTint);
        //     scatteredRay.color = scatterTint;
        //     return scatteredRay;
        // }

};