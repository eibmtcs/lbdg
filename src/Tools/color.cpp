#include <string>
#include <glm/glm.hpp>

#include "color.hpp"

Color::Color(std::string name, int code, int valueR, int valueG, int valueB, int edgeR, int edgeG, int edgeB, int alpha)
    :name(name), code(code), alpha(alpha)
{
    float base = 1.0f/255.0f;
    value = glm::vec4( valueR * base, valueG * base, valueB * base, alpha * base);
    edge = glm::vec4( edgeR * base, edgeG * base, edgeB * base, alpha * base);
}

Color &Color::setValue(int r, int g, int b, int alpha)
{
    float base = 1.0f/255.0f;
    value = glm::vec4( r * base, g * base, b * base, alpha * base);
    return *this;
}

Color &Color::setEdge(int r, int g, int b, int alpha)
{
    float base = 1.0f/255.0f;
    edge = glm::vec4( r * base, g * base, b * base, alpha * base);
    return *this;
}

glm::vec4 Color::getValue()
{
    return value;
}

glm::vec4 Color::getValue() const
{
    return value;
}


glm::vec4 Color::getEdge()
{
    return edge;
}
