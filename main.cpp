#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include "camera.h"
#include <string>
#include "cubemap.h"
#include "sphere.h"
#include "renderer.h"
#include "light.h"
#include "plane.h"
#define _USE_MATH_DEFINES

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    Eigen::Matrix4d pose = Eigen::Matrix4d::Identity();

    std::cout << pose << std::endl;
    Camera camera(pose, 400, 400, 200);
    Eigen::Transform<double, 3, Eigen::Affine> rot(Eigen::AngleAxisd(-M_PI*0.5, Eigen::Vector3d(1, 0, 0)));
    Eigen::Matrix4d t = Eigen::Matrix4d::Identity();
    camera.pose.block<3,3>(0,0) = rot*camera.pose.block<3,3>(0,0);
    
    Cubemap cubemap("skybox.png");
    Sphere sphere(Eigen::Vector3d(-3, 8, 0), 3);
    Sphere sphere2(Eigen::Vector3d(3, 10, 0), 3);
    Plane plane(Eigen::Vector3d(0,0,1), Eigen::Vector3d(0,0,-3));
    sphere.setMaterial(Material(cv::Vec3d(0, 0, 100), cv::Vec3d(0, 0, 0), 0));
    sphere2.setMaterial(Material(cv::Vec3d(0, 0, 0), cv::Vec3d(1, 1, 1), 0));
    plane.setMaterial(Material(cv::Vec3d(100, 100, 100), cv::Vec3d(0, 0, 0), 1));
    Light pl(Eigen::Vector3d(-10,0,10), cv::Vec3d(2, 2 , 2));
    Renderer renderer(1);
    renderer.addLight(&pl);
    renderer.addObject(&cubemap);
    renderer.addObject(&sphere);
    renderer.addObject(&sphere2);
    renderer.addObject(&plane);
    renderer.render(camera, 1);
    
    return 0;
}