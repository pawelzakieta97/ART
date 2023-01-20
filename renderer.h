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
#include "ctpl.h"

// float gaussCurve(float mean, float std, float x){
//     return 1/(std * sqrt(2 * M_PI)) * exp(-0.5*pow((x - mean)/std, 2));
// }

float colorBrightness(Eigen::Vector3d color){
    return pow((color[0] * color[0] + color[1] * color[1] + color[2] * color[2])/3, 0.5);
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

        void render(Camera camera, int superSampling=1, int depth=2){
            auto start = std::chrono::high_resolution_clock::now();
            auto rays = camera.project(superSampling);
            cv::Mat image(camera.height, camera.width, CV_64FC3, cv::Scalar(0,0,0));
            int max_updates = 100;
            int show_every = rays.size() / max_updates;
            int ray_index = 0;
            for (Ray ray: rays){
                auto min_color = rayColor(ray, depth);
                // ray.color = min_color;
                auto point_idx = camera.getPixelIndex(ray);
                auto c = (min_color/superSampling/superSampling) * 255;
                image.at<cv::Vec3d>(point_idx) = image.at<cv::Vec3d>(point_idx) + cv::Vec3d(c[0], c[1], c[2]);
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
        void renderMultithreaded(Camera camera, int superSampling=1, int depth=2, int numThreads=10){
            auto start = std::chrono::high_resolution_clock::now();
            auto rays = camera.project(superSampling);
            cv::Mat image(camera.height, camera.width, CV_64FC3, cv::Scalar(0,0,0));
            int max_updates = 100;
            int show_every = rays.size() / max_updates;
            int ray_index = 0;
            ctpl::thread_pool threadPool(numThreads);
            std::vector<std::future<void>> results(rays.size());
            std::vector<Eigen::Vector3d> rayColors(rays.size());
            for (Ray ray: rays){
                results[ray_index] = threadPool.push([&ray, rayColors, depth, &image, this, &camera, superSampling, ray_index](int){
                    auto pixelColor = Eigen::Vector3d(0.5, 0.5, 0.5); //rayColor(ray, depth);
                    //auto point_idx = camera.getPixelIndex(ray);
                    ray.color = pixelColor;
                    // rayColors[ray_index] = (pixelColor/superSampling/superSampling) * 255;
                    //  image.at<cv::Vec3d>(point_idx) + cv::Vec3d(c[0], c[1], c[2]);
                });
                ray_index += 1;
            }
            for (int i=0; i < results.size(); ++i){
                results[i].get();
                auto point_idx = camera.getPixelIndex(rays[i]);
                auto c = rays[i].color;
                image.at<cv::Vec3d>(point_idx) + cv::Vec3d(c[0], c[1], c[2]);
                if (i % show_every == 0){
                    cv::Mat img_show;
                    image.convertTo(img_show, CV_8UC3);
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
        Eigen::Vector3d rayColor(Ray ray, int bounces=1){
            float min_distance;
            auto minObject = hit(ray, min_distance);
            if (minObject == nullptr){
                return Eigen::Vector3d(0,0,0);
            }
            Eigen::Vector3d hitLocation = ray.getHitLocation(min_distance);
            Eigen::Vector3d hitColor = minObject->getDiffuse(hitLocation);
            if (min_distance == 696969) return hitColor;
            Eigen::Vector3d hitReflectiveness = minObject->getReflectiveness(hitLocation);
            float hitRoughness = minObject->getRoughness(hitLocation);
            Eigen::Vector3d normal = minObject->getNormal(hitLocation);
            Ray reflectedRay(hitLocation, ray.direction - 2 * normal * normal.dot(ray.direction), Eigen::Vector3d(0,0,0));

            Eigen::Vector3d color(0,0,0);
            for (auto light: lights){
                Eigen::Vector3d p2l = -light->getLightToPoint(hitLocation);
                float distanceToLight = p2l.norm();
                Ray point2light(hitLocation, p2l/distanceToLight, Eigen::Vector3d(1,1,1));
                float distance;
                if (hit(point2light, distance)==nullptr || distance > distanceToLight){
                    Ray lightRay = light->getLightRay(hitLocation);
                    auto phongColor = minObject -> material -> phong(ray, reflectedRay , normal, lightRay);
                    color += phongColor;
                }
                
            }
            if (bounces > 1){
                Ray scatteredRay = minObject -> scatter(ray, hitLocation);
                auto scatteredColor = rayColor(scatteredRay, bounces-1);
                if (scatteredColor[0] + scatteredColor[1] + scatteredColor[2] > 10){
                    rayColor(scatteredRay, bounces-1);
                }
                color += multiplyColor(scatteredRay.color, scatteredColor);
            }
            return color;
        }

};