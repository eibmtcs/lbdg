#include "render.hpp"
#include "subFile.hpp"
#include "function.hpp"
#include <sstream>
#include <vector>
#include <stack>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstddef>
#include <algorithm>

#include <iostream>
#include <string>
#include <memory>

#include "glm/gtx/string_cast.hpp"
#include "fileget.hpp"
#include "color.hpp"
#include "subFile.hpp"
#include "line.hpp"
#include "print.hpp"

using std::vector;
using std::string;
using std::shared_ptr;

using std::cout;
using std::endl;

/* float base = 1.0f/255.0f; */
/* std::map<int, Color> colorMap = { */
/*     { 0, Color(" 0",  0,  27,  42,  52, 128, 128, 128)}, */
/*     { 1, Color(" 1",  1,  30,  90, 168,  51,  51,  51)}, */
/*     { 4, Color(" 4",  4, 180,   0,   0,  51,  51,  51)}, */
/*     { 7, Color(" 7",  7, 138, 146, 141,  51,  51,  51)}, */
/*     {16, Color("16", 16, 255, 255, 128,  51,  51,  51)}, */
/*     {24, Color("24", 24, 127, 127, 127,  51,  51,  51)}, */
/*     {36, Color("36", 36, 201,  26,   9, 102,  13,   5, 128)}, */
/*     {39, Color("39", 39, 193, 223, 240, 111, 180, 220, 128)}, */
/*     {46, Color("46", 46, 245, 205,  47, 180, 146,   8, 128)}, */
/* }; */

Fileget file(1000000);
std::map<int, Color> colorMap;
std::map<std::string, std::shared_ptr<Subfile>> partMap;
std::map<std::string, std::pair<std::vector<glm::mat4>, std::pair<std::vector<glm::vec4>, std::vector<glm::vec4>>>> indeces;

std::stack<glm::mat4> location(std::deque<glm::mat4>{glm::mat4(1.0f)});
std::stack<int> color;
Point max, min;

int manualControl = 0;

std::string modelName;

bool firstDraw = true;


int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        string tmp(argv[i]);
        if (tmp == "-c") {
            if (i+1 < argc) {
                modelName = argv[++i];
            } else {
                /* std::cout << "error" << std::endl; */
            }
        }
        if (tmp == "-m")
            manualControl = 1;
    }
    
    if (modelName == "") {
        std::cout
            << "lbdg: missing file operand\n"
            << "Try 'lbdg --help' for more information.\n"
            << std::endl;
        return 0;
    }


    colorMap = makeColorMap(file.openLDConfig());
    Render render;
    render.setShader("~/lbdg/shader/simple/shader.vs", "~/lbdg/shader/simple/shader.fs");

    std::string modelTmp;
    std::string tmp = modelName.substr(modelName.size() - 3, 3);
    transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);

    if (tmp == "mpd") {
        modelTmp = openMPDFile(file.open(modelName));
    } else {
        modelTmp = file.open(modelName);
    }
    vector<std::string> partVector;

    std::istringstream filetmp(modelTmp);
    std::string line;
    std::string part;
    while (std::getline(filetmp, line)) {
        if (line.substr(0, 6) == "0 STEP") {
            partVector.push_back(part);
            part = "";
        } else
            part = part + '\n' + line;
    }

    modelName = modelName.substr(0, modelName.find_last_of("."));
    modelName = modelName.substr(modelName.find_last_of("/") + 1, modelName.size());

    std::cout << "modelName: " << modelName << std::endl;

    mkdir(modelName.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    mkdir((modelName + "/images").c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    /* std::cout << modelTmp << std::endl; */

    /* auto model = makeModel(modelTmp); */
    /* for (auto &i : model) */
    /*     i->show(Shader(), glm::mat4(1.0f), glm::mat4(1.0f)); */
    /* firstDraw = false; */

    /* std::cout << modelName << std::endl; */
    /* for (auto &i : model) */
    /*     i->makeModelTreeList(); */

    /* auto map_it = indeces.cbegin(); */
    /* while (map_it != indeces.cend()) { */
    /*     std::cerr << map_it->first << ":" << std::endl; */
    /*     for (auto &i : map_it->second) */
    /*         std::cerr << i.second << std::endl; */
    /*     ++map_it; */
    /* } */

    /* render.show(model, modelName); */
    /* max.debug(); */
    /* min.debug(); */
    /* model.push_back(std::make_shared<Line>(1, max.x, min.y, max.z, max.x, max.y, max.z)); */
    /* model.push_back(std::make_shared<Line>(1, min.x, min.y, max.z, min.x, max.y, max.z)); */
    /* model.push_back(std::make_shared<Line>(1, max.x, min.y, max.z, min.x, min.y, max.z)); */
    /* model.push_back(std::make_shared<Line>(1, max.x, max.y, max.z, min.x, max.y, max.z)); */

    /* model.push_back(std::make_shared<Line>(1, max.x, min.y, min.z, max.x, max.y, min.z)); */
    /* model.push_back(std::make_shared<Line>(1, min.x, min.y, min.z, min.x, max.y, min.z)); */
    /* model.push_back(std::make_shared<Line>(1, max.x, min.y, min.z, min.x, min.y, min.z)); */
    /* model.push_back(std::make_shared<Line>(1, max.x, max.y, min.z, min.x, max.y, min.z)); */

    /* model.push_back(std::make_shared<Line>(1, max.x, min.y, min.z, max.x, min.y, max.z)); */
    /* model.push_back(std::make_shared<Line>(1, max.x, max.y, min.z, max.x, max.y, max.z)); */
    /* model.push_back(std::make_shared<Line>(1, min.x, min.y, min.z, min.x, min.y, max.z)); */
    /* model.push_back(std::make_shared<Line>(1, min.x, max.y, min.z, min.x, max.y, max.z)); */

    /* render.show(model, modelName); */
    std::vector<std::vector<std::pair<std::string, int>>> thumbnail;
    std::vector<std::shared_ptr<Model>> total;
    for (int i = 0; i < partVector.size(); i++) {
        auto model = makeModel(partVector[i]);
        /* std::cerr << partVector[i] << std::endl; */
        total.insert(total.end(), model.begin(), model.end());
        vector<std::pair<std::string ,int>> tmp;
        for (auto &r : model) {
            if (r->type == SUBFILE) {
                /* std::cerr << r->partName << std::endl; */
                decltype(tmp.size()) j;
                for (j = 0; j < tmp.size(); j++)
                    if (tmp[j].first == r->partName) {
                        /* std::cerr << r->partName << std::endl; */
                        break;
                    }
                if (j == tmp.size())
                    tmp.push_back({r->partName, 1});
                else
                    tmp[j].second++;
                /* std::cerr << j << std::endl; */
            }
        }

        /* for (auto &r : tmp) */
        /*     std::cerr << r.first << "     " << r.second << std::endl; */
        /* std::cerr << "------------------" << std::endl; */

        thumbnail.push_back(tmp);

        firstDraw = true;
        for (auto &i : total)
            i->show(Shader(), glm::mat4(1.0f), glm::mat4(1.0f));
        firstDraw = false;
        if (manualControl == 1)
            render.show(total, modelName  + " [" + std::to_string(i+1) + " - " + std::to_string(partVector.size()) + "]");
        else
            render.makePNG(total, modelName  + " [" + std::to_string(i+1) + " - " + std::to_string(partVector.size()) + "]");
        /* indeces.empty(); */
    }

    Print a(modelName, thumbnail);
    a.makeHtml();

    return EXIT_SUCCESS;
}
