#include <opencv2/highgui.hpp>
#include <iostream>
#include "Eigen/Dense"
#include <string>
#include "ray.h"
#include <math.h>
#include "renderable.h"
#include "camera.h"
#include <memory>
#include "light.h"
#include <random>
#include <chrono>


// float gaussCurve(float mean, float std, float x){
//     return 1/(std * sqrt(2 * M_PI)) * exp(-0.5*pow((x - mean)/std, 2));
// }

float colorBrightness(cv::Vec3d color){
    return pow((color.val[0] * color.val[0] + color.val[1] * color.val[1] + color.val[2] * color.val[2])/3, 0.5);
}

class Renderer{
    public:
        int maxBounces;
        int samples = 20;
        std::vector<std::shared_ptr<Renderable>> objects;
        std::random_device rd{};
        std::mt19937 gen{rd()};
        Renderer(int maxBounces){
            this->maxBounces = maxBounces;
        }
        std::vector<std::shared_ptr<Light>> lights;

        void render(Camera camera, int superSampling=1, int depth=3){
            auto start = std::chrono::high_resolution_clock::now();
            auto rays = camera.project(superSampling);
            cv::Mat image(camera.height, camera.width, CV_64FC3, cv::Scalar(0,0,0));
            int i = 0;
            int max_updates = 100;
            int show_every = rays.size() / max_updates;
            int ray_index = 0;
            for (Ray ray: rays){
                auto min_color = rayColor(ray, depth);
                ray.color = min_color;
                auto point_idx = camera.getPixelIndex(ray);
                image.at<cv::Vec3d>(point_idx) = image.at<cv::Vec3d>(point_idx) + (min_color/superSampling/superSampling) * 255;
                i += 1;
                ray_index += 1;
                if (ray_index % show_every == 0){
                    cv::Mat img_show;
                    image.convertTo(img_show, CV_8UC3);
                    auto px2 = img_show.at<cv::Vec3d>(point_idx);
                    cv::imshow("rendered image", img_show);
                    cv::waitKey(1);
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
            std::cout<<duration.count()<< std::endl;
            cv::waitKey(0);
        }
        void addObject(Renderable* object){
            objects.push_back(std::shared_ptr<Renderable>(object));
        }

        void addLight(Light* light){
            lights.push_back(std::shared_ptr<Light>(light));
        }
        std::shared_ptr<Renderable> hit(Ray ray, float& distance){
            float min_distance = 9999999.0;
            std::shared_ptr<Renderable> minObject=nullptr;
            for (auto object: objects){
                if (object->hitDistance(ray, distance)){
                    if (distance > 0 && distance < min_distance){
                        min_distance = distance;
                        minObject = object;
                    }
                }
                
            }
            distance = min_distance;
            return minObject;
        }
        cv::Vec3d rayColor(Ray ray, int bounces=1){
            float min_distance;
            auto minObject = hit(ray, min_distance);
            if (minObject == nullptr){
                return cv::Vec3d(0,0,0);
            }
            Eigen::Vector3d hitLocation = ray.getHitLocation(min_distance);
            cv::Vec3d hitColor = minObject->getDiffuse(hitLocation);
            if (min_distance == 696969) return hitColor;
            cv::Vec3d hitReflectiveness = minObject->getReflectiveness(hitLocation);
            float hitRoughness = minObject->getRoughness(hitLocation);
            Eigen::Vector3d normal = minObject->getNormal(hitLocation);
            Ray reflectedRay(hitLocation, ray.direction - 2 * normal * normal.dot(ray.direction), cv::Vec3d(0,0,0));

            cv::Vec3d color;
            for (auto light: lights){
                Eigen::Vector3d p2l = -light->getLightToPoint(hitLocation);
                float distanceToLight = p2l.norm();
                Ray point2light(hitLocation, p2l/distanceToLight, cv::Vec3d(1,1,1));
                float distance;
                if (hit(point2light, distance)==nullptr || distance > distanceToLight){
                     Ray lightRay = light->getLightRay(hitLocation);
                    auto phongColor = minObject -> material -> phong(ray, reflectedRay , normal, lightRay);
                    color += phongColor;
                }
                
            }
            if (bounces > 1){
                int diffuseSamples = colorBrightness(hitColor/255) / (colorBrightness(hitColor/255) + colorBrightness(hitReflectiveness)) * samples;
                int specularSamples = samples - diffuseSamples;
                diffuseSamples = 0;
                specularSamples = 0;
                //diffuseSamples = 0;
                Ray scatteredRay = minObject -> scatter(ray, hitLocation);
                color += scatteredRay.color.mul(rayColor(scatteredRay, bounces - 1));


                for (int i=0; i<diffuseSamples; i++){
                    // diffuse secondary bounces
                    Ray diffuseRay = bounceSample(Ray(hitLocation, normal, cv::Vec3d(0, 0, 0)), normal, 1);
                    
                    color += (rayColor(diffuseRay, bounces - 1)/samples).mul(hitColor/255);
                }
                // specular secondary bounces

                if (hitReflectiveness.val[0] || hitReflectiveness.val[1] || hitReflectiveness.val[2]){
                    for (int i=0; i<specularSamples; i++){
                        Ray specularRay = bounceSample(reflectedRay, normal, hitRoughness);
                        color += (rayColor(specularRay, bounces - 1)/samples).mul(hitReflectiveness);
                    }
                }
                
            }
            return color;
        }
        
        Ray bounceSample(Ray bouncedRay, Eigen::Vector3d normal, float roughness){
            Ray ray;
            ray.start = bouncedRay.start;
            if (!roughness){
                ray.direction = bouncedRay.direction;
            }
            else{
                std::normal_distribution<> d{0,roughness};
                float parallelAngleDeviation = d(gen);
                // normalAngleDeviation = 0;
                float currentAngle = acos(bouncedRay.direction.dot(normal)-0.00001);
                if (currentAngle + parallelAngleDeviation > M_PI/2){//} || currentAngle + normalAngleDeviation > M_PI/2){
                    parallelAngleDeviation -= currentAngle + parallelAngleDeviation - M_PI/2;
                }

                Eigen::Vector3d parallelRotationAxis = normal.cross(bouncedRay.direction);
                // perfect reflection ray bounces along normal
                if (parallelRotationAxis.norm() < 0.01){
                    parallelRotationAxis = normal.cross(Eigen::Vector3d(1,0,0));
                }
                if (parallelRotationAxis.norm() < 0.01){
                    parallelRotationAxis = normal.cross(Eigen::Vector3d(0,1,0));
                }
                Eigen::Transform<double, 3, Eigen::Affine> normalRotation(Eigen::AngleAxisd(parallelAngleDeviation, parallelRotationAxis));

                ray.direction = normalRotation * bouncedRay.direction;
                float axialAngle = d(gen);
                // axialAngle = 0;
                Eigen::Transform<double, 3, Eigen::Affine> axialRotation(Eigen::AngleAxisd(axialAngle, bouncedRay.direction));
                ray.direction = axialRotation * ray.direction;
            }
            return ray;
        }

};