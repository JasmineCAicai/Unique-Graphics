//
// Created by LEI XU on 5/16/19.
//

#ifndef ASSIGNMENT6_INTERSECTION_H
#define ASSIGNMENT6_INTERSECTION_H
#include "Vector.h"
#include "Material.h"
class Object;
class Sphere;

struct Intersection
{
    Intersection(){
        happened=false;
        coords=Vector3f();
        normal=Vector3f();
        distance=std::numeric_limits<double>::max();
        obj=nullptr;
        m=nullptr;
    }
    bool happened;
    Vector3f coords;
    Vector3f normal;
    double distance;
    Object* obj;
    Material* m;
};
#endif //RAYTRACING_INTERSECTION_H
