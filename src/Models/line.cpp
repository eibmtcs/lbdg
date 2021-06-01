#include <map>
#include <memory>
#include <stack>

#include "model.hpp"
#include "subFile.hpp"
#include "point.hpp"

#include "line.hpp"

Line::Line(Point a, Point b)
{
    type = LINE;
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //Point vertices[2] = {a,b};
    vertices = std::vector<Point>{a, b};
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 2, &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));

    glEnableVertexAttribArray(0);
}

Line::Line(int color, float x1, float y1, float z1, float x2, float y2, float z2) : Line(Point(x1, y1, z1), Point(x2, y2, z2))
{
    colorCode = color;
}

Line::Line(int color, std::string s)
{
    float x1, y1, z1, x2, y2, z2;
    std::istringstream read(s);
    read >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    new(this) Line(color, x1, y1, z1, x2, y2, z2);
}

void Line::show(Shader shader, glm::mat4 projection, glm::mat4 view)
{
    extern std::stack<int> color;
    extern std::stack<glm::mat4> location;
    extern Point max, min;
    glm::vec4 a1 = location.top() * glm::vec4((glm::vec3)vertices[0], 1.0);
    glm::vec4 b1 = location.top() * glm::vec4((glm::vec3)vertices[1], 1.0);
    glm::vec3 a = glm::vec3(a1.x/a1.w, a1.y/a1.w, a1.z/a1.w);
    glm::vec3 b = glm::vec3(b1.x/b1.w, b1.y/b1.w, b1.z/b1.w);

    if (a.x > max.x) max.x = a.x;
    if (a.y > max.y) max.y = a.y;
    if (a.z > max.z) max.z = a.z;
    if (a.x < min.x) min.x = a.x;
    if (a.y < min.y) min.y = a.y;
    if (a.z < min.z) min.z = a.z;
    if (b.x > max.x) max.x = b.x;
    if (b.y > max.y) max.y = b.y;
    if (b.z > max.z) max.z = b.z;
    if (b.x < min.x) min.x = b.x;
    if (b.y < min.y) min.y = b.y;
    if (b.z < min.z) min.z = b.z;


    if (colorCode == 16 || colorCode == 24) {
        shader.set("color", colorMap[color.top()].getEdge());
    } else {
        shader.set("color", colorMap[colorCode].getEdge());
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
}
