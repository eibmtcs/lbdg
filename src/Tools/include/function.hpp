#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <map>

#include "color.hpp"

class Model;

glm::mat4 makeMat(float x, float y, float z, float a, float b, float c, float d, float e, float f, float g, float h, float i);
glm::mat4 makeMat(std::string s);

std::vector<std::shared_ptr<Model>> makeModel(std::string s);

std::map<int, Color> makeColorMap(std::string s);

void saveRender2Png(int width, int height, std::string name);

std::string openMPDFile(std::string s);

#endif
