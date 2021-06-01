#ifndef __LINE_HPP__
#define __LINE_HPP__

#include <map>

#include "point.hpp"
#include "model.hpp"
#include "subFile.hpp"
#include "point.hpp"

extern std::map<int, Color> colorMap;

class Line : public Model {
public:
    Line(Point a, Point b);
    Line(int color, float x1, float y1, float z1, float x2, float y2, float z2);
    Line(int color, std::string s);
    void show(Shader shader, glm::mat4 projection, glm::mat4 view);

    std::vector<Point> vertices;
};

#endif
