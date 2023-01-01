#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include "ray.h"

class Camera{
    public:
        Eigen::Matrix4d pose;
        int width;
        int height;
        float f;
        Camera();
        Camera(Eigen::Matrix4d pose, int width, int height, float f);
        // {
        //     this-> pose = pose;
        //     this->width = width;
        //     this->height = height;
        //     this->f = f;
        // }
        std::vector<Ray> project(int superSampling){
            std::vector<Ray> v;
            Eigen::Vector4d forward = pose * Eigen::Vector4d(0,0,1,0);
            Eigen::Matrix3Xd rays = getRays(superSampling);
            Eigen::Vector3d pos = pose.block<3,1>(0, 3);
            rays = pose.block<3,3>(0,0) * rays;
            for (int i=0; i<width * height * superSampling * superSampling; i++){
                // std::cout<<i<<std::endl;
                v.push_back(Ray(pos, rays.block<3,1>(0, i), cv::Vec3d(1,1,1)));
            }
            return v;
        }
        
        Eigen::Matrix3Xd getRays(int superSampling){
            Eigen::Matrix3Xd result(3, width * height*  superSampling * superSampling);
            int i = 0;

            for (int y=-height/2 * superSampling; y<height/2 * superSampling; y++){
                for (int x=-width/2 * superSampling; x<width/2 * superSampling; x ++){
                    float pixel_x = (float)x / superSampling + 1/(float(superSampling*2));
                    float pixel_y = (float)y / superSampling + 1/(float(superSampling*2));
                    Eigen::Vector3d ray_direction(pixel_x / f, pixel_y / f, 1);
                    result.block<3,1>(0, i) = ray_direction/ray_direction.norm();
                    i ++;
                }
            }
            return result;
        }
        cv::Point getPixelIndex(Ray ray){
            Eigen::Vector3d relativeRayDirection = pose.inverse().block<3,3>(0, 0) * ray.direction;
            float x = relativeRayDirection.x() / relativeRayDirection.z() * f + width/2;
            float y = relativeRayDirection.y() / relativeRayDirection.z() * f + height/2;
            return cv::Point(int(x), int(y));
        }
        void rotateYaw(double angle){
            Eigen::Transform<double, 3, Eigen::Affine> rot(Eigen::AngleAxisd(angle, Eigen::Vector3d(0, 0, 1)));
            Eigen::Matrix4d t = Eigen::Matrix4d::Identity();
            pose.block<3,3>(0,0) = rot*pose.block<3,3>(0,0);
        }

        void rotatePitch(double angle){
            Eigen::Transform<double, 3, Eigen::Affine> rot(Eigen::AngleAxisd(angle, Eigen::Vector3d(1, 0, 0)));
            Eigen::Matrix4d t = Eigen::Matrix4d::Identity();
            pose.block<3,3>(0,0) = rot*pose.block<3,3>(0,0);
        }

        void move(double x, double y, double z){
            pose.coeffRef(0, 3) +=  x;
            pose.coeffRef(1, 3) += y;
            pose.coeffRef(2, 3) += z;
        }

};