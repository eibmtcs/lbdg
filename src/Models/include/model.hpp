#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <vector>
#include <memory>
#include "point.hpp"
#include "shader.hpp"

class Subfile;

enum Type {
    COMMAND,
    SUBFILE,
    LINE,
    TRIANGLE,
    QUADRILATERAL,
    OPTIONALLINE,
};

class Model {
public:
    Model();
    virtual void show(Shader shader, glm::mat4 projection, glm::mat4 view) = 0;
    virtual void makeModelTreeList();

    Type type;
    unsigned int VAO;
    std::vector<Point> vertices;
    std::shared_ptr<Subfile> parent;
    int colorCode;
    std::string partName;
};

#endif
