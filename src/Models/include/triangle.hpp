#ifndef __TRIANGLE_HPP__
#define __TRIANGLE_HPP__

#include <memory>
#include "model.hpp"
#include "subFile.hpp"

class Triangle : public Model {
public:
    Triangle(Point a, Point b, Point c);
    Triangle(int color, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    Triangle(int color, std::string s);
    void show(Shader shader, glm::mat4 projection, glm::mat4 view);
};

#endif
