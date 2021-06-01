#ifndef __SUBFILE_HPP__
#define __SUBFILE_HPP__

#include "shader.hpp"
#include "point.hpp"
#include "model.hpp"
#include "color.hpp"

#include <map>

extern std::map<int, Color> colorMap;

class Subfile : public Model {
public:
    Subfile() : Model() {}
    Subfile(glm::mat4 e);
    Subfile(std::string s);
    Subfile(int color, glm::mat4 e, std::string partContent, bool construct);
    Subfile(int color, float x, float y, float z, float a, float b, float c, float d, float e, float f, float g, float h, float i, std::string partContent, bool construct);
    void show(Shader shader, glm::mat4 projection, glm::mat4 view);
    void showme(Shader shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model2, int colorCode2);
    void makeModelTreeList();

    std::vector<std::shared_ptr<Model>> modelTree;
    glm::mat4 model;
};

#endif
