#include <stack>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "subFile.hpp"
#include "function.hpp"
#include "fileget.hpp"

Subfile::Subfile(glm::mat4 e) : Subfile() {
    type = SUBFILE;
    model = e;
}

Subfile::Subfile(std::string s) : Subfile() {
    model = makeMat(s);
}

Subfile::Subfile(int color, glm::mat4 e, std::string partName, bool construct) : Subfile(e) {
    colorCode = color;
    this->partName = partName;
    if (construct) {
        extern Fileget file;
        std::string tmp = file.openParts(partName);
        if (!tmp.empty())
            modelTree = makeModel(tmp);
    }
}

Subfile::Subfile(int color, float x, float y, float z, float a, float b, float c, float d, float e, float f, float g, float h, float i, std::string partName, bool construct) : Subfile(color, makeMat(x, y, z, a, b, c, d, e, f, g, h, i), partName, construct) {
}

void Subfile::showme(Shader shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model2, int colorCode2)
{
    extern std::stack<int> color;
    extern std::map<std::string, std::shared_ptr<Subfile>> partMap;
    shader.set("model", glm::scale(model2, glm::vec3(0.9999f, 0.9999f, 0.9999f)));
    color.push(colorCode2);

    if (partMap.find(partName) != partMap.end() && !partMap[partName]->modelTree.empty()) {
        for (auto &i : partMap[partName]->modelTree)
            if (i->type != SUBFILE)
                i->show(shader, projection, view);
    }

    color.pop();
    shader.set("model", glm::scale(model2, glm::vec3(0.9999f, 0.9999f, 0.9999f)));
}

void Subfile::show(Shader shader, glm::mat4 projection, glm::mat4 view)
{
    extern std::stack<glm::mat4> location;
    extern std::map<std::string, std::shared_ptr<Subfile>> partMap;
    extern std::map<int, Color> colorMap;
    extern std::stack<int> color;
    extern std::map<std::string, std::pair<std::vector<glm::mat4>, std::pair<std::vector<glm::vec4>, std::vector<glm::vec4>>>> indeces;

    extern bool firstDraw;

    bool pushColor = (colorCode != 16 && colorCode != 24);
    if (pushColor)
        color.push(colorCode);
    location.push(location.top() * model);

    if (firstDraw) {
        if (modelTree.empty()) {
            if (indeces.find(partName) == indeces.end()) {
                indeces.insert({partName, std::pair<std::vector<glm::mat4>, std::pair<std::vector<glm::vec4>, std::vector<glm::vec4>>>
                        {std::vector<glm::mat4>{location.top()}, {std::vector<glm::vec4>{colorMap[color.top()].getEdge()}, std::vector<glm::vec4>{colorMap[color.top()].getValue()}}}});
            } else {
                indeces.find(partName)->second.first.push_back(glm::scale(location.top(), glm::vec3(0.999f, 0.999f, 0.999f)));
                indeces.find(partName)->second.second.first.push_back(colorMap[color.top()].getEdge());
                indeces.find(partName)->second.second.second.push_back(colorMap[color.top()].getValue());
            }
        }
    }

    if (modelTree.empty()) {
        if (partMap.find(partName) != partMap.end() && !partMap[partName]->modelTree.empty()) {
            partMap[partName]->show(shader, projection, view);
        }
    } else {
        for (auto &i : modelTree)
            i->show(shader, projection, view);
    }

    location.pop();
    if (pushColor)
        color.pop();
}

std::stack<std::string> blank({""});
void Subfile::makeModelTreeList()
{
    extern std::map<std::string, std::shared_ptr<Subfile>> partMap;
    extern std::stack<std::string> blank;
    if (modelTree.empty()) {
        blank.push(blank.top() + " │");
        std::string tmp = blank.top();
        std::cout << tmp.substr(0, tmp.find_last_of("│")-2) + "├" << partName << std::endl;
    }

    if (modelTree.empty()) {
        if (partMap.find(partName) != partMap.end() && !partMap[partName]->modelTree.empty())
            partMap[partName]->makeModelTreeList();
    } else
        for (auto &i : modelTree)
            i->makeModelTreeList();

    if (modelTree.empty()) {
        blank.pop();
    }
}
