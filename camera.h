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
        std::vector<Ray> project(int samples){
            std::vector<Ray> v;
            Eigen::Vector4d forward = pose * Eigen::Vector4d(0,0,1,0);
            Eigen::Matrix3Xd rays = getRays(samples);
            Eigen::Vector3d pos = pose.block<3,1>(0, 3);
            rays = pose.block<3,3>(0,0) * rays;
            for (int i=0; i<width * height * samples; i++){
                // std::cout<<i<<std::endl;
                v.push_back(Ray(pos, rays.block<3,1>(0, i), cv::Vec3d(1,1,1)));
            }
            return v;
        }
        Eigen::Matrix3Xd getRays(int samples){
            Eigen::Matrix3Xd result(3, width*height*samples);
            int i = 0;
            for (int y=-height/2; y<height/2; y++){
                for (int x=-width/2; x<width/2; x ++){
                    for (int s=0; s<samples; s++){
                        Eigen::Vector3d ray_direction((float)x / f, (float)y / f, 1);
                        result.block<3,1>(0, i) = ray_direction/ray_direction.norm();
                        i ++;
                    }
                }
            }
            return result;
        }
        // cv::Mat draw(Cubemap cubemap){
        //     rays = 
        // }
};