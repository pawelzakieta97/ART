#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include "ray.h"
#include "random.h"
#include "color.h"

float gaussCurve(float mean, float std, float x){
    return 1/(std * sqrt(2 * M_PI)) * exp(-0.5*pow((x - mean)/std, 2));
}

class Material{
    public:
        Eigen::Vector3d diffuse;
        Eigen::Vector3d reflectiveness;
        Ray scatter(const Ray& ray, const Eigen::Vector3d& hitPoint, const Eigen::Vector3d& normal){
            Eigen::Vector3d diffuseDirection = normal + randomUnitVector();
            diffuseDirection /= diffuseDirection.norm();
            // eventually sample tint color based on hit location / UV coordinates
            Eigen::Vector3d reflectedDirection = ray.direction - 2 * normal * normal.dot(ray.direction);
            Eigen::Vector3d scatteredDirection = (1-roughness) * reflectedDirection + roughness * diffuseDirection;
            if (roughness == 1){
                int a = 1;
            }
            Eigen::Vector3d tint = diffuse * roughness + reflectiveness * (1-roughness);
            Ray scatteredRay(hitPoint, scatteredDirection, tint);
            return scatteredRay;
        }
        Eigen::Vector3d phong(const Ray& ray, const Ray& reflectedRay, const Eigen::Vector3d& normal, const Ray& lightRay){
            Eigen::Vector3d hitLocation = reflectedRay.start;
            float cosAngle = (-lightRay.direction).dot(normal);
            Eigen::Vector3d color(0,0,0);
            if (cosAngle < 0.001) {
                return color;
            }
            color = multiplyColor(lightRay.color, diffuse) * cosAngle;
            //skip specular?
            cosAngle = (-lightRay.direction).dot(reflectedRay.direction);
            if (cosAngle < 0.001) {
                return color;
            }
            // color += multiplyColor(lightRay.color, reflectiveness) * gaussCurve(1, roughness+0.001, cosAngle);
            if (roughness==1 || roughness==0) return color;
            color += multiplyColor(lightRay.color, reflectiveness) * pow(cosAngle, 1/roughness);
            //color += Eigen::Vector3d(reflectedRay.direction.x()*127 + 127, reflectedRay.direction.x()*127 + 127, reflectedRay.direction.x()*127 + 127);
            return color;
        }
        float roughness;
        Material(Eigen::Vector3d diffuse, Eigen::Vector3d reflectiveness, float roughness){
            this->roughness = roughness;
            this->reflectiveness = reflectiveness;
            this->diffuse = diffuse;
        };
        Material(){
            diffuse[0] = 0;
            diffuse[1] = 0;
            diffuse[2] = 0;
            roughness = 0.1;
            reflectiveness[0] = 0;
            reflectiveness[1] = 0;
            reflectiveness[2] = 0;
        }
};