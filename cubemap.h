#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include <string>
#include "ray.h"
#include <math.h>
#include "renderable.h"

class Cubemap: public Renderable{
    cv::Mat map;
    cv::Mat front;
    cv::Mat back;
    cv::Mat left;
    cv::Mat right;
    cv::Mat top;
    cv::Mat bottom;
    int height;
    int width;
    public:
        Cubemap(std::string path){
            map = cv::imread(path,cv::IMREAD_COLOR);
            height = map.rows / 3;
            width = map.cols / 4;
            front = map(cv::Rect(width, height, width, height));
            back = map(cv::Rect(3 * width, height, width, height));
            left = map(cv::Rect(0, height, width, height));
            right = map(cv::Rect(2 * width, height, width,height));
            top = map(cv::Rect(width, 0, width, height));
            bottom = map(cv::Rect(width, 2 * height, width, height));
        }
        cv::Vec3d getDiffuse(Eigen::Vector3d location){
            Ray ray(Eigen::Vector3d(0,0,0), location / location.norm(), cv::Vec3d(1,1,1));
            if (ray.direction.x() >= abs(ray.direction.y()) && ray.direction.x() >= abs(ray.direction.z())){
                int y = (0.70710678118 - ray.direction.z())/1.41421356237 * height;
                int x = (0.70710678118 - ray.direction.y())/1.41421356237 * height;
                return cv::Vec3d(right.at<cv::Vec3b>(y, x));
            }
            else if (ray.direction.x() <= -abs(ray.direction.y()) && ray.direction.x() <= -abs(ray.direction.z())){
                int y = (0.70710678118 - ray.direction.z())/1.41421356237 * height;
                int x = (0.70710678118 + ray.direction.y())/1.41421356237 * height;
                return cv::Vec3d(left.at<cv::Vec3b>(y, x));
            }
            else if (ray.direction.y() >= abs(ray.direction.x()) && ray.direction.y() >= abs(ray.direction.z())){
                int y = (0.70710678118 - ray.direction.z())/1.41421356237 * height;
                int x = (0.70710678118 + ray.direction.x())/1.41421356237 * height;
                return cv::Vec3d(front.at<cv::Vec3b>(y, x));
            }
            else if (ray.direction.y() <= -abs(ray.direction.x()) && ray.direction.y() <= -abs(ray.direction.z())){
                int y = (0.70710678118 - ray.direction.z())/1.41421356237 * height;
                int x = (0.70710678118 - ray.direction.x())/1.41421356237 * height;
                return cv::Vec3d(back.at<cv::Vec3b>(y, x));
            }
            else if (ray.direction.z() <= -abs(ray.direction.x()) && ray.direction.z() <= -abs(ray.direction.y())){
                int y = (0.70710678118 - ray.direction.y())/1.41421356237 * height;
                int x = (0.70710678118 + ray.direction.x())/1.41421356237 * height;
                return cv::Vec3d(bottom.at<cv::Vec3b>(y, x));
            }
            else if (ray.direction.z() >= abs(ray.direction.x()) && ray.direction.z() >= abs(ray.direction.y())){
                int y = (0.70710678118 + ray.direction.y())/1.41421356237 * height;
                int x = (0.70710678118 + ray.direction.x())/1.41421356237 * height;
                return cv::Vec3d(top.at<cv::Vec3b>(y, x));
            }
            return cv::Vec3d(0,0,0);
        }
        cv::Vec3d getReflectiveness(Eigen::Vector3d location){return cv::Vec3d(0,0,0);}
        bool hitDistance(Ray ray, float& distance){
            distance = 696969;
            return true;
        };
};