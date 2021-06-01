#ifndef __POINT_HPP__
#define __POINT_HPP__

#include <iostream>
#include <glm/glm.hpp>

class Point {

public:
    Point(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Point &operator=(Point &rhs);
    Point &operator=(const Point &rhs);
    Point &setXyz(float x, float y, float z);
    operator glm::vec3() const;
    float getX();
    float getY();
    float getZ();
    void debug();

    float x;
    float y;
    float z;
};

#endif
