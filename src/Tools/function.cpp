#include <cctype>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <map>
#include <stack>
#include <pngwriter.h>

#include "function.hpp"
#include "fileget.hpp"
#include "subFile.hpp"
#include "line.hpp"
#include "triangle.hpp"
#include "quadrilateral.hpp"
#include "optionalLine.hpp"

glm::mat4 makeMat(float x, float y, float z, float a, float b, float c, float d, float e, float f, float g, float h, float i)
{
    float mat[16] = {
        a, d, g, 0,
        b, e, h, 0,
        c, f, i, 0,
        x, y, z, 1,
    };

    return glm::make_mat4(mat);
}

glm::mat4 makeMat(std::string s) {
    float x, y, z, a, b, c, d, e, f, g, h, i;
    std::istringstream read(s);
    read >> x >> y >> z >> a >> b >> c >> d >> e >> f >> g >> h >> i;
    return makeMat(x, y, z, a, b, c, d, e, f, g, h, i);
}

std::vector<std::shared_ptr<Model>> makeModel(std::string s)
{
    std::vector<std::shared_ptr<Model>> model;
    std::istringstream fullText(s);
    std::string part;
    extern Fileget file;
    extern std::map<std::string, std::shared_ptr<Subfile>> partMap;
    bool firstMpdPart = true;

    while (std::getline(fullText, part)) {
        /* std::cerr << part << std::endl; */
        int type = 0, color;
        std::string pointarr;
        std::istringstream line(part);
        line >> type;
        if (type != 0) line >> color;
        if (type == 0) {
            std::string comment, partname;
            line >> comment;
            if (comment == "FILE") {
                std::getline(line, partname);
                size_t n = partname.find_last_not_of(" \r\n\t");
                partname.erase(n+1, s.size() - n);
                n = partname.find_first_not_of(" \r\n\t");
                partname.erase(0, n);
                n = partname.find_last_of(".");
                for (int i = n; i < partname.size(); i++)
                    partname[i] = std::tolower(partname[i]);
                for (auto &c : partname)
                    if (c == '\\')
                        c = '/';
                /* std::cerr << comment << "  " << partname << std::endl; */
                if (!firstMpdPart) {
                    std::string partInMpd;
                    while (std::getline(fullText, part)) {
                        std::istringstream line(part);
                        int type = 0;
                        std::string comment;
                        /* std::cerr << part << std::endl; */
                        line >> type >> comment;
                        if (type == 0 && comment == "FILE") {
                            if (partMap.find(partname) != partMap.end()) {
                                partMap.find(partname)->second->modelTree = makeModel(partInMpd);
                                if (partMap.find(partname)->second->modelTree.empty() == false)
                                    for (auto &i : partMap.find(partname)->second->modelTree)
                                          i->parent = partMap.find(partname)->second;
                            } else {
                                auto addPart = std::make_shared<Subfile>(16, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, partname, false);
                                addPart->modelTree = makeModel(partInMpd);
                                if (addPart->modelTree.empty() == false)
                                    for (auto &i : addPart->modelTree)
                                          i->parent = addPart;
                                partMap.insert({partname, addPart});
                            }
                            partInMpd = "";

                            std::getline(line, partname);
                            size_t n = partname.find_last_not_of(" \r\n\t");
                            partname.erase(n+1, s.size() - n);
                            n = partname.find_first_not_of(" \r\n\t");
                            partname.erase(0, n);
                            n = partname.find_last_of(".");
                            for (int i = n; i < partname.size(); i++)
                                partname[i] = std::tolower(partname[i]);
                            for (auto &c : partname)
                                if (c == '\\')
                                    c = '/';
                        } else {
                            partInMpd = partInMpd + '\n' + part;
                        }
                    }
                    if (partMap.find(partname) != partMap.end()) {
                        partMap.find(partname)->second->modelTree = makeModel(partInMpd);
                        if (partMap.find(partname)->second->modelTree.empty() == false)
                            for (auto &i : partMap.find(partname)->second->modelTree)
                                  i->parent = partMap.find(partname)->second;
                    } else {
                        auto addPart = std::make_shared<Subfile>(16, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, partname, false);
                        addPart->modelTree = makeModel(partInMpd);
                        if (addPart->modelTree.empty() == false)
                            for (auto &i : addPart->modelTree)
                                  i->parent = addPart;
                        partMap.insert({partname, addPart});
                    }
                }
                firstMpdPart = false;
            }
        } else if (type == 1) {
            float x, y, z, a, b, c, d, e, f, g, h, i;
            std::string partname;
            line >> x >> y >> z >> a >> b >> c >> d >> e >> f >> g >> h >> i;
            std::getline(line, partname);
            size_t n = partname.find_last_not_of(" \r\n\t");
            partname.erase(n+1, s.size() - n);
            n = partname.find_first_not_of(" \r\n\t");
            partname.erase(0, n);
            n = partname.find_last_of(".");
            for (int i = n; i < partname.size(); i++)
                partname[i] = std::tolower(partname[i]);
            for (auto &c : partname)
                if (c == '\\')
                    c = '/';

            if (partMap.find(partname) == partMap.end()) {
                auto addPart = std::make_shared<Subfile>(16, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, partname, true);
                if (addPart->modelTree.empty() == false) {
                    for (auto &i : addPart->modelTree)
                          i->parent = addPart;
                } else {
                }
                partMap.insert({partname, addPart});
                if (partMap.find(partname) == partMap.end()) {
                    std::cerr << "can't find partname : " << partname << std::endl;
                }
            }

            /* extern std::stack<glm::mat4> location; */
            glm::mat4 model2 = makeMat(x, y, z, a, b, c, d, e, f, g, h, i);
            /* location.push(location.top() * model2); */

            auto subfile = std::make_shared<Subfile>(color, model2, partname, false);

            /* location.pop(); */

            model.push_back(subfile);
        } else if (type == 2) {
            std::getline(line, pointarr);
            model.push_back(std::make_shared<Line>(color, pointarr));
        } else if (type == 3) {
            std::getline(line, pointarr);
            model.push_back(std::make_shared<Triangle>(color, pointarr));
        } else if (type == 4) {
            std::getline(line, pointarr);
            model.push_back(std::make_shared<Quadrilateral>(color, pointarr));
        } else if (type == 5) {
            std::getline(line, pointarr);
            model.push_back(std::make_shared<OptionalLine>(color, pointarr));
        } else {
        }
    }
    return model;
}

std::map<int, Color> makeColorMap(std::string s)
{
    std::map<int, Color> colorMap;
    std::istringstream fullText(s);
    std::string part;

    while(std::getline(fullText, part)) {
        std::istringstream line(part);
        int type; std::string define;
        line >> type >> define;
        if (type == 0 && define == "!COLOUR") {
            int valueR, valueG, valueB, edgeR, edgeG, edgeB;
            int code, alpha = 255, luminance;
            std::string key, name, value, edge, material;

            line >> name;
            while (line >> key) {
                if (key == "CODE") {
                    line >> code;
                } else if (key == "VALUE") {
                    line >> value;
                } else if (key == "EDGE") {
                    line >> edge;
                } else if (key == "ALPHA") {
                    line >> alpha;
                } else if (key == "LUMINANCE") {
                    line >> luminance;
                } else if (key == "CHROME") {
                } else if (key == "PEARLESCENT") {
                } else if (key == "RUBBER") {
                } else if (key == "MATTE_METALLIC") {
                } else if (key == "METAL") {
                } else if (key == "MATERIAL") {
                }
            }
            /* std::cout << std::setw(35) << name << std::setw(10) << code << std::setw(10) << value << std::setw(10) << edge << std::setw(10) << alpha << std::endl;; */
            std::istringstream(value.substr(1,2)) >> std::hex >> valueR;
            std::istringstream(value.substr(3,2)) >> std::hex >> valueG;
            std::istringstream(value.substr(5,2)) >> std::hex >> valueB;
            std::istringstream(edge.substr(1,2)) >> std::hex >> edgeR;
            std::istringstream(edge.substr(3,2)) >> std::hex >> edgeG;
            std::istringstream(edge.substr(5,2)) >> std::hex >> edgeB;

            colorMap.insert({code, Color(name, code, valueR, valueG, valueB, edgeR, edgeG, edgeB, alpha)});
        }
    }

    return colorMap;
}

void saveRender2Png(int width, int height, std::string name)
{
    pngwriter png(width, height, 1.0, name.c_str());
    int size = 3 * width * height;
    GLfloat *pixels = new GLfloat[size];
    size_t x = 1, y = 1;
    double r, g, b;

    glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, pixels);
    for (size_t i = 0; i < size; i++)
        switch (i % 3) {
            case 2:
                b = pixels[i];
                break;
            case 1:
                g = pixels[i];
                break;
            case 0:
                r = pixels[i];
                png.plot(x, y, r, g, b);
                if (x == width)
                    x = 1, y++;
                else
                    x++;
                break;
        }
    png.close();
}

std::string openMPDFile(std::string s)
{
    std::istringstream fullText(s);
    std::string mpdFile, partname, part;
    std::string mainPart, otherPart;

    extern Fileget file;
    extern std::map<std::string, std::shared_ptr<Subfile>> partMap;
    int mpdPart = 0;

    while (std::getline(fullText, part)) {
        int type = 0;
        std::string pointarr;
        std::istringstream line(part);
        line >> type;
        if (type == 0) {
            std::string comment;
            line >> comment;
            if (comment == "FILE") {
                if (mpdPart == 1) {
                    /* std::cerr << mainPart << std::endl; */
                } else {
                    if (partname != "") {
                        if (partMap.find(partname) != partMap.end()) {
                            partMap.find(partname)->second->modelTree = makeModel(otherPart);
                            if (partMap.find(partname)->second->modelTree.empty() == false)
                                for (auto &i : partMap.find(partname)->second->modelTree)
                                      i->parent = partMap.find(partname)->second;
                        } else {
                            std::shared_ptr<Subfile> addPart = std::make_shared<Subfile>(16, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, std::string("abc"), false);
                            addPart->modelTree = makeModel(otherPart);
                            if (addPart->modelTree.empty() == false)
                                for (auto &i : addPart->modelTree)
                                      i->parent = addPart;
                            partMap.insert({partname, addPart});
                        }
                    }

                    /* std::cerr << otherPart << std::endl; */
                    otherPart = "";
                }
                std::getline(line, partname);
                size_t n = partname.find_last_not_of(" \r\n\t");
                partname.erase(n+1, s.size() - n);
                n = partname.find_first_not_of(" \r\n\t");
                partname.erase(0, n);
                n = partname.find_last_of(".");
                for (int i = n; i < partname.size(); i++)
                    partname[i] = std::tolower(partname[i]);
                for (auto &c : partname)
                    if (c == '\\')
                        c = '/';
                /* std::cerr << comment << "  " << partname << std::endl; */

                mpdPart++;
                /* std::cerr << "-------------" << partname << "-------------" << std::endl; */
            } else {
                if (mpdPart == 1) {
                    mainPart += part + '\n';
                } else {
                    otherPart += part + '\n';
                }
            }
        } else {
            if (mpdPart == 1) {
                mainPart += part + '\n';
            } else {
                otherPart += part + '\n';
            }
        }
    }
    if (partname != "") {
        if (partMap.find(partname) != partMap.end()) {
            partMap.find(partname)->second->modelTree = makeModel(otherPart);
            if (partMap.find(partname)->second->modelTree.empty() == false)
                for (auto &i : partMap.find(partname)->second->modelTree)
                      i->parent = partMap.find(partname)->second;
        } else {
            std::shared_ptr<Subfile> addPart = std::make_shared<Subfile>(16, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, std::string("abc"), false);
            addPart->modelTree = makeModel(otherPart);
            if (addPart->modelTree.empty() == false)
                for (auto &i : addPart->modelTree)
                      i->parent = addPart;
            partMap.insert({partname, addPart});
        }
    }
    /* std::cerr << otherPart << std::endl; */
    return mainPart;
}
