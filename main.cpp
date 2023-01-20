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
#include "environment.h"

#define _USE_MATH_DEFINES

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    Eigen::Matrix4d pose = Eigen::Matrix4d::Identity();
    
    std::cout << pose << std::endl;
    // pose.coeffRef(2,3) += 10;
    // std::cout << pose << std::endl;
    Camera camera(pose, 800, 800, 500);

    camera.rotatePitch(-M_PI*0.55);
    camera.move(0, -3, 5);
    // Eigen::Transform<double, 3, Eigen::Affine> rot(Eigen::AngleAxisd(-M_PI*0.5, Eigen::Vector3d(1, 0, 0)));
    // camera.pose.block<3,3>(0,0) = rot*camera.pose.block<3,3>(0,0);
    Cubemap cubemap("skybox.png");
    Environment environment;
    Sphere sphere(Eigen::Vector3d(-4, 8, 0), 3);
    Sphere sphere2(Eigen::Vector3d(4, 9, 0), 3);
    Sphere sphere3(Eigen::Vector3d(0, 14, 0), 3);
    Plane plane(Eigen::Vector3d(0,0,1), Eigen::Vector3d(0,0,-3));
    
    Material redDiffuse = Material(Eigen::Vector3d(0.0, 0.0, 0.5), Eigen::Vector3d(0.0, 0.0, 0.0), 1);
    Material goldMetal = Material(Eigen::Vector3d(0.0, 0.35, 0.5), Eigen::Vector3d(0.1, 0.35, 0.5), 0.1);
    Material mirror = Material(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(1, 1, 1), 0);
    Material gray = Material(Eigen::Vector3d(0.4, 0.4, 0.4), Eigen::Vector3d(0.0, 0.0, 0.0), 1);
    
    sphere.setMaterial(&goldMetal);
    sphere2.setMaterial(&mirror);
    sphere3.setMaterial(&redDiffuse);
    plane.setMaterial(&gray);
    PointLight pl(Eigen::Vector3d(-10,0,10), Eigen::Vector3d(200, 200 , 200));
    SunLight sl(Eigen::Vector3d(-10,0,10), Eigen::Vector3d(1, 1 , 1));
    Renderer renderer(1);
    renderer.addLight(&sl);
    renderer.addObject(&plane);
    // renderer.addObject(&cubemap);
    renderer.addObject(&environment);
    renderer.addObject(&sphere);
    renderer.addObject(&sphere2);
    renderer.addObject(&sphere3);

    int numSpheres = 40;
    // Sphere * spheres;
    // Material * materials;
    std::vector<Sphere> spheres;
    std::vector<Material> materials;
    for (int i=0; i<numSpheres; i++){
        double x = (randomDouble() - 0.5) * 100 - 20;
        // x = 0;
        double y = randomDouble() * 200 + 10;
        // y = 10;
        double roughness = randomDouble();
        double R = (randomDouble()+1)*2;
        // r = 5;
        double g = randomDouble();
        double r = randomDouble();
        double b = randomDouble();
        Sphere newSphere(Eigen::Vector3d(x, y, R-3), R);
        Material newMaterial = Material(Eigen::Vector3d(r*0.7, g*0.7, b*0.7), Eigen::Vector3d(r, g, b), roughness*roughness*roughness);
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


    renderer.render(camera, 3, 4);
    
    return 0;
}