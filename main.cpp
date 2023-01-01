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
#include "material.h"
#include "random.h"
#define _USE_MATH_DEFINES

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    Eigen::Matrix4d pose = Eigen::Matrix4d::Identity();
    
    std::cout << pose << std::endl;
    // pose.coeffRef(2,3) += 10;
    // std::cout << pose << std::endl;
    Camera camera(pose, 400, 400, 200);

    camera.rotatePitch(-M_PI*0.55);
    camera.move(0, 0, 5);
    // Eigen::Transform<double, 3, Eigen::Affine> rot(Eigen::AngleAxisd(-M_PI*0.5, Eigen::Vector3d(1, 0, 0)));
    // camera.pose.block<3,3>(0,0) = rot*camera.pose.block<3,3>(0,0);
    Cubemap cubemap("skybox.png");

    Sphere sphere(Eigen::Vector3d(-3, 8, 0), 3);
    Sphere sphere2(Eigen::Vector3d(3, 10, 0), 3);
    Plane plane(Eigen::Vector3d(0,0,1), Eigen::Vector3d(0,0,-3));
    
    Material redDiffuse = Material(cv::Vec3d(0.0, 0.0, 0.5), cv::Vec3d(0.1, 0.1, 0.5), 0.1);
    Material mirror = Material(cv::Vec3d(0, 0, 0), cv::Vec3d(1, 1, 1), 0);
    Material gray = Material(cv::Vec3d(0.4, 0.4, 0.4), cv::Vec3d(0, 0, 0), 1);
    
    sphere.setMaterial(&redDiffuse);
    sphere2.setMaterial(&mirror);
    plane.setMaterial(&gray);
    PointLight pl(Eigen::Vector3d(-10,0,10), cv::Vec3d(200, 200 , 200));
    Renderer renderer(1);
    renderer.addLight(&pl);
    renderer.addObject(&cubemap);
    renderer.addObject(&sphere);
    renderer.addObject(&sphere2);
    renderer.addObject(&plane);

    int numSpheres = 20;
    // Sphere * spheres;
    // Material * materials;
    std::vector<Sphere> spheres;
    std::vector<Material> materials;
    for (int i=0; i<numSpheres; i++){
        double x = (randomDouble() - 0.5) * 100;
        // x = 0;
        double y = randomDouble() * 100 + 10;
        // y = 10;
        double roughness = randomDouble();
        double R = (randomDouble()+1)*2;
        // r = 5;
        double g = randomDouble();
        double r = randomDouble();
        double b = randomDouble();
        Sphere newSphere(Eigen::Vector3d(x, y, R-3), R);
        Material newMaterial = Material(cv::Vec3d(r, g, b), cv::Vec3d(r, g, b), roughness*roughness*roughness);
        spheres.push_back(newSphere);
        materials.push_back(newMaterial);
        
    }
    auto it_sphere = spheres.begin();
    auto it_material = materials.begin();
    for(; it_sphere != spheres.cend(); ){
        it_sphere -> setMaterial(&(*it_material));
        renderer.addObject(&(*it_sphere));
        it_sphere ++;
        it_material ++;
    }


    renderer.render(camera, 1);
    
    return 0;
}