#ifndef __OPTIONALLINE_HPP__
#define __OPTIONALLINE_HPP__

#include <map>

#include "point.hpp"
#include "model.hpp"
#include "subFile.hpp"

extern std::map<int, Color> colorMap;

class OptionalLine : public Model {
public:
    OptionalLine(Point a, Point b, Point c, Point d);
    OptionalLine(int color, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4);
    OptionalLine(int color, std::string s);
    void show(Shader shader, glm::mat4 projection, glm::mat4 view);
};

#endif
