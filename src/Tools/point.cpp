#include <iostream>
#include "point.hpp"

Point &Point::operator=(Point &rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

Point &Point::operator=(const Point &rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

Point &Point::setXyz(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    return *this;
}

void Point::debug()
{
    std::cout << "x: " << x << "  y: " << y << "  z: " << z << std::endl;
}

Point::operator glm::vec3() const
{
    return glm::vec3(this->x, this->y, this->z);
}

float Point::getX()
{
    return this->x;
}

float Point::getY()
{
    return this->y;
}

float Point::getZ()
{
    return this->z;
}
