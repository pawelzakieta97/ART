#include "camera.h"

Camera::Camera(Eigen::Matrix4d pose, int width, int height, float f)
{
    this-> pose = pose;
    this->width = width;
    this->height = height;
    this->f = f;
}
Camera::Camera()
{
    this-> pose = Eigen::Matrix4d::Identity();
    this->width = 100;
    this->height = 100;
    this->f = 100;
}