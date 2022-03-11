//
// Created by LEI XU on 5/16/19.
//

#ifndef ASSIGNMENT6_BVH_H
#define ASSIGNMENT6_BVH_H

#include <atomic>
#include <vector>
#include <memory>
#include <ctime>
#include "Object.h"
#include "Ray.h"
#include "Bounds3.h"
#include "Intersection.h"
#include "Vector.h"

struct BVHBuildNode;
// BVHAccel Forward Declarations
struct BVHPrimitiveInfo;

// BVHAccel Declarations
inline int leafNodes, totalLeafNodes, totalPrimitives, interiorNodes;
class BVHAccel {

public:
    // BVHAccel Public Types
    enum class SplitMethod { NAIVE, SAH };

    // BVHAccel Public Methods
    BVHAccel(std::vector<Object*> p, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::NAIVE);
    Bounds3 WorldBound() const;
    ~BVHAccel();

    Intersection Intersect(const Ray &ray) const;
    Intersection getIntersection(BVHBuildNode* node, const Ray& ray)const;
    bool IntersectP(const Ray &ray) const;
    BVHBuildNode* root;

    // BVHAccel Private Methods
    BVHBuildNode* recursiveBuild(std::vector<Object*>objects);

    // BVHAccel Private Data
    const int maxPrimsInNode;
    const SplitMethod splitMethod;
    std::vector<Object*> primitives;
};

struct BVHBuildNode {
    Bounds3 bounds;
    BVHBuildNode *left;
    BVHBuildNode *right;
    Object* object;

public:
    int splitAxis=0, firstPrimOffset=0, nPrimitives=0;
    // BVHBuildNode Public Methods
    BVHBuildNode(){
        bounds = Bounds3();
        left = nullptr;right = nullptr;
        object = nullptr;
    }
};




#endif //RAYTRACING_BVH_H
