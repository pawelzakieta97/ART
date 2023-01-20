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
        Eigen::Vector3d getDiffuse(Eigen::Vector3d location){
            // return Eigen::Vector3d(0.5, 0.5, 0.5);
            Ray ray(Eigen::Vector3d(0,0,0), location / location.norm(), Eigen::Vector3d(1,1,1));
            if (ray.direction.x() >= abs(ray.direction.y()) && ray.direction.x() >= abs(ray.direction.z())){
                int y = (0.70710678118 - ray.direction.z())/1.41421356237 * height;
                int x = (0.70710678118 - ray.direction.y())/1.41421356237 * height;
                auto px = right.at<cv::Vec3b>(y, x);
                // return Eigen::Vector3d(right.at<cv::Vec3b>(y, x)) / 255.0;
                return Eigen::Vector3d(px.val[0], px.val[1], px.val[2]) / 255.0;
            }
            else if (ray.direction.x() <= -abs(ray.direction.y()) && ray.direction.x() <= -abs(ray.direction.z())){
                int y = (0.70710678118 - ray.direction.z())/1.41421356237 * height;
                int x = (0.70710678118 + ray.direction.y())/1.41421356237 * height;
                // return Eigen::Vector3d(left.at<cv::Vec3b>(y, x)) / 255.0;
                auto px = left.at<cv::Vec3b>(y, x);
                return Eigen::Vector3d(px.val[0], px.val[1], px.val[2]) / 255.0;
            }
            else if (ray.direction.y() >= abs(ray.direction.x()) && ray.direction.y() >= abs(ray.direction.z())){
                int y = (0.70710678118 - ray.direction.z())/1.41421356237 * height;
                int x = (0.70710678118 + ray.direction.x())/1.41421356237 * height;
                // return Eigen::Vector3d(front.at<cv::Vec3b>(y, x)) / 255.0;
                auto px = front.at<cv::Vec3b>(y, x);
                return Eigen::Vector3d(px.val[0], px.val[1], px.val[2]) / 255.0;
            }
            else if (ray.direction.y() <= -abs(ray.direction.x()) && ray.direction.y() <= -abs(ray.direction.z())){
                int y = (0.70710678118 - ray.direction.z())/1.41421356237 * height;
                int x = (0.70710678118 - ray.direction.x())/1.41421356237 * height;
                // return Eigen::Vector3d(back.at<cv::Vec3b>(y, x)) / 255.0;
                auto px = back.at<cv::Vec3b>(y, x);
                return Eigen::Vector3d(px.val[0], px.val[1], px.val[2]) / 255.0;
            }
            else if (ray.direction.z() <= -abs(ray.direction.x()) && ray.direction.z() <= -abs(ray.direction.y())){
                int y = (0.70710678118 - ray.direction.y())/1.41421356237 * height;
                int x = (0.70710678118 + ray.direction.x())/1.41421356237 * height;
                // return Eigen::Vector3d(bottom.at<cv::Vec3b>(y, x)) / 255.0;
                auto px = bottom.at<cv::Vec3b>(y, x);
                return Eigen::Vector3d(px.val[0], px.val[1], px.val[2]) / 255.0;
            }
            else if (ray.direction.z() >= abs(ray.direction.x()) && ray.direction.z() >= abs(ray.direction.y())){
                int y = (0.70710678118 + ray.direction.y())/1.41421356237 * height;
                int x = (0.70710678118 + ray.direction.x())/1.41421356237 * height;
                // return Eigen::Vector3d(top.at<cv::Vec3b>(y, x)) / 255.0;
                auto px = top.at<cv::Vec3b>(y, x);
                return Eigen::Vector3d(px.val[0], px.val[1], px.val[2]) / 255.0;
            }
            return Eigen::Vector3d(0,0,0);
        }
        Eigen::Vector3d getReflectiveness(Eigen::Vector3d location){return Eigen::Vector3d(0,0,0);}
        bool hitDistance(Ray ray, float& distance){
            distance = 696969;
            return true;
        };
};