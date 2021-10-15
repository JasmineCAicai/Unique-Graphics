//
// Created on 2021/10/15.
//

#ifndef ASSIGNMENT5_LIGHT_H
#define ASSIGNMENT5_LIGHT_H

#include "Vector.h"

class Light
{
public:
    Light(const Vector3f& p, const Vector3f& i)
            : position(p)
            , intensity(i)
    {}
    virtual ~Light() = default;
    Vector3f position;
    Vector3f intensity;
};


#endif //ASSIGNMENT5_LIGHT_H
