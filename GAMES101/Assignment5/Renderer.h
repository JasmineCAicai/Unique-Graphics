//
// Created on 2021/10/15.
//

#ifndef ASSIGNMENT5_RENDERER_H
#define ASSIGNMENT5_RENDERER_H

#include "Scene.h"

struct hit_payload
{
    float tNear;
    uint32_t index;
    Vector2f uv;
    Object* hit_obj;
};

class Renderer
{
public:
    void Render(const Scene& scene);

private:
};

#endif //ASSIGNMENT5_RENDERER_H
