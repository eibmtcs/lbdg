#ifndef __QUADRILATERAL_HPP__
#define __QUADRILATERAL_HPP__

#include <memory>

#include "model.hpp"
#include "subFile.hpp"

class Quadrilateral : public Model {
public:
    Quadrilateral(Point a, Point b, Point c, Point d);
    Quadrilateral(int color, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4);
    Quadrilateral(int color, std::string s);
    void show(Shader shader, glm::mat4 projection, glm::mat4 view);
};

#endif
