#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include "ray.h"
#include "random.h"

float gaussCurve(float mean, float std, float x){
    return 1/(std * sqrt(2 * M_PI)) * exp(-0.5*pow((x - mean)/std, 2));
}

class Material{
    public:
        cv::Vec3d diffuse;
        cv::Vec3d reflectiveness;
        Ray scatter(const Ray& ray, const Eigen::Vector3d& hitPoint, const Eigen::Vector3d& normal){
            Eigen::Vector3d diffuseDirection = normal + randomUnitVector();
            diffuseDirection /= diffuseDirection.norm();
            // eventually sample tint color based on hit location / UV coordinates
            Eigen::Vector3d reflectedDirection = ray.direction - 2 * normal * normal.dot(ray.direction);
            Eigen::Vector3d scatteredDirection = (1-roughness) * reflectedDirection + roughness * diffuseDirection;
            cv::Vec3d tint = diffuse * roughness + reflectiveness * (1-roughness);
            Ray scatteredRay(hitPoint, scatteredDirection, tint);
            return scatteredRay;
        }
        cv::Vec3d phong(const Ray& ray, const Ray& reflectedRay, const Eigen::Vector3d& normal, const Ray& lightRay){
            Eigen::Vector3d hitLocation = reflectedRay.start;
            float cosAngle = (-lightRay.direction).dot(normal);
            cv::Vec3d color;
            if (cosAngle > 0) {
                color = lightRay.color.mul(diffuse) * cosAngle;
            }
            //skip specular?
            cosAngle = (-lightRay.direction).dot(reflectedRay.direction);
            color += lightRay.color.mul(reflectiveness) * gaussCurve(1, roughness+0.001, cosAngle);
            //color += cv::Vec3d(reflectedRay.direction.x()*127 + 127, reflectedRay.direction.x()*127 + 127, reflectedRay.direction.x()*127 + 127);
            return color;
        }
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