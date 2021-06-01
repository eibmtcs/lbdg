#include <map>
#include <memory>
#include <stack>

#include "model.hpp"
#include "subFile.hpp"
#include "point.hpp"

#include "optionalLine.hpp"

OptionalLine::OptionalLine(Point a, Point b, Point c, Point d) {
    type = OPTIONALLINE;
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    vertices = std::vector<Point>{a, b, c, d};
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 2, &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));

    glEnableVertexAttribArray(0);
}

OptionalLine::OptionalLine(int color, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4)
    : OptionalLine(Point(x1, y1, z1), Point(x2, y2, z2), Point(x3, y3, z3), Point(x4, y4, z4)) {
    colorCode = color;
}

OptionalLine::OptionalLine(int color, std::string s) {
    float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    std::istringstream read(s);
    read >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3 >> x4 >> y4 >> z4;
    new(this) OptionalLine(color, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);
}

void OptionalLine::show(Shader shader, glm::mat4 projection, glm::mat4 view) {
    extern std::stack<glm::mat4> location;
    extern std::stack<int> color;
    glm::mat4 model = location.top();

    glm::vec4 a1 = projection * view * model * glm::vec4((glm::vec3)vertices[0], 1.0);
    glm::vec4 b1 = projection * view * model * glm::vec4((glm::vec3)vertices[1], 1.0);
    glm::vec4 c1 = projection * view * model * glm::vec4((glm::vec3)vertices[2], 1.0);
    glm::vec4 d1 = projection * view * model * glm::vec4((glm::vec3)vertices[3], 1.0);
    glm::vec3 a = glm::vec3(a1.x/a1.w, a1.y/a1.w, a1.z/a1.w);
    glm::vec3 b = glm::vec3(b1.x/b1.w, b1.y/b1.w, b1.z/b1.w);
    glm::vec3 c = glm::vec3(c1.x/c1.w, c1.y/c1.w, c1.z/c1.w);
    glm::vec3 d = glm::vec3(d1.x/d1.w, d1.y/d1.w, d1.z/d1.w);
    if (((c.x - a.x) / (b.x - a.x) - (c.y - a.y) / (b.y - a.y)) > 0
            == ((d.x - a.x) / (b.x - a.x) - (d.y - a.y) / (b.y - a.y)) > 0) {

        if (colorCode == 16 || colorCode == 24)
            shader.set("color", colorMap[color.top()].getEdge());
        else
            shader.set("color", colorMap[colorCode].getEdge());

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
    }
}
