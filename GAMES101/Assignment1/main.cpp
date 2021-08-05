#include "Triangle.h"
#include "rasterizer.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <Eigen/Eigen>
#include <cmath>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
            -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

// 逐个元素地构建模型变换矩阵并返回该矩阵
Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    // 4x4矩阵
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    float rad = rotation_angle * (MY_PI/180);
    float sin_theta = sin(rad);
    float cos_theta = cos(rad);

    // 绕z轴旋转，z不变
    // X x Y -> Z
    /*
     * cos  -sin    0    0
     * sin   cos    0    0
     *  0     0     1    0
     *  0     0     0    1
     */
    model(0,0) = cos_theta;
    model(0,1) = -sin_theta;
    model(1,0) = sin_theta;
    model(1,1) = cos_theta;

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f ortho = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f persp_ortho = Eigen::Matrix4f::Identity();

    float l, r, n, f, t, b;
    n = zNear;
    f = zFar;
    t = n * tan((eye_fov/2) * (MY_PI/180));
    b = -t;
    r = t * aspect_ratio;
    l = -r;

    ortho(0,0) = 2 / (r - l);
    ortho(0,3) = - (r + l) / 2;
    ortho(1,1) = 2 / (t - b);
    ortho(1,3) = - (t + b) / 2;
    ortho(2,2) = 2 / (n - f);
    ortho(2,3) = - (n + f) / 2;

    persp_ortho(0,0) = n;
    persp_ortho(1,1) = n;
    persp_ortho(2,2) = n + f;
    persp_ortho(2,3) = - n * f;
    persp_ortho(3,2) = 1;
    persp_ortho(3,3) = 0;

    projection = ortho * persp_ortho;

    return projection;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}