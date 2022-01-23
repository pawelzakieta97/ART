#include "ray.h"

Ray::Ray(Eigen::Vector3d start, Eigen::Vector3d direction, cv::Vec3d color){
    this->start = start;
    this->direction = direction;
    this->color = color;
}

Ray::Ray(){
    this->start = Eigen::Vector3d();
    this->direction = Eigen::Vector3d(0,0,1);
    this->color = cv::Vec3b(1,1,1);
}

Eigen::Vector3d Ray::getHitLocation(float distance){
    return this->start + this->direction * distance;
}