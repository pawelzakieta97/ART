#pragma once
#include <opencv2/highgui.hpp>
#include <math.h>
#include <iostream>
#include "Eigen/Dense"
#include "renderable.h"
#include "material.h"
#include "ray.h"

class Sphere: public Renderable{
    public:
        Eigen::Vector3d position;
        float radius;
        Sphere(Eigen::Vector3d position, float radius){
            this->position = position;
            this->radius = radius;
        }
        bool hitDistance(Ray ray, float& distance){
            auto v = ray.start-position;
            float dx = v.x();
            float dy = v.y();
            float dz = v.z();
            float b = 2 * (ray.direction.x()*dx + ray.direction.y() * dy + ray.direction.z()*dz);
            float delta = pow(b, 2) - 4 * (dx*dx + dy*dy + dz*dz - radius*radius);
            if (delta>0){
                float distance1 = (-b - pow(delta, 0.5)) / 2;
                float distance2 = (-b + pow(delta, 0.5)) / 2;
                if (distance2<0.001){
                    return false;
                }
                distance = distance1;
                return true;
            }
            return false;
        }
        Eigen::Vector3d getNormal(Eigen::Vector3d location){
            auto r = location - position;
            return r / r.norm();
        }
        

};