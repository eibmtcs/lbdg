#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <glm/glm.hpp>
#include <string>

enum finishes {
    CHROME,
    PEARLESCENT,
    RUBBER,
    MATTE_METALLIC,
    METAL,
    METERIAL,
};

class Color {
public:
    Color() = default;
    Color(std::string name, int code, int valueR, int valueG, int valueB, int edgeR, int edgeG, int edgeB, int alpha = 255);
    Color &setValue(int r, int g, int b, int alpha = 255);
    Color &setEdge(int r, int g, int b, int alpha = 255);
    glm::vec4 getValue();
    glm::vec4 getValue() const;
    glm::vec4 getEdge();


private:
    std::string name;
    int code;
    glm::vec4 value;
    glm::vec4 edge;
    int alpha;
    int luminance;
    finishes material;
};

#endif
