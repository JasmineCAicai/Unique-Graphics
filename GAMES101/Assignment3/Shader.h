//
// Created by 蔡淑娴 on 2021/8/13.
//

#ifndef ASSIGNMENT3_SHADER_H
#define ASSIGNMENT3_SHADER_H

#include <Eigen/Eigen>
#include "Texture.h"


struct fragment_shader_payload
{
    fragment_shader_payload()
    {
        texture = nullptr;
    }

    fragment_shader_payload(const Eigen::Vector3f& col, const Eigen::Vector3f& nor,const Eigen::Vector2f& tc, Texture* tex) :
            color(col), normal(nor), tex_coords(tc), texture(tex) {}


    Eigen::Vector3f view_pos;
    Eigen::Vector3f color;
    Eigen::Vector3f normal;
    Eigen::Vector2f tex_coords;
    Texture* texture;
};

struct vertex_shader_payload
{
    Eigen::Vector3f position;
};

#endif //ASSIGNMENT3_SHADER_H
