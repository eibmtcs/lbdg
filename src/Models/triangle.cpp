#include <memory>
#include <stack>
#include "model.hpp"
#include "subFile.hpp"

#include "triangle.hpp"
#include "point.hpp"

Triangle::Triangle(Point a, Point b, Point c)
{
    type = TRIANGLE;
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    Point vertices[] = { a, b, c};
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 3, &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));

    glEnableVertexAttribArray(0);
}

Triangle::Triangle(int color, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) : Triangle(Point(x1, y1, z1), Point(x2, y2, z2), Point(x3, y3, z3))
{
    colorCode = color;
}

Triangle::Triangle(int color, std::string s)
{
    float x1, y1, z1, x2, y2, z2, x3, y3, z3;
    std::istringstream read(s);
    read >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
    new(this) Triangle(color, x1, y1, z1, x2, y2, z2, x3, y3, z3);
}

void Triangle::show(Shader shader, glm::mat4 projection, glm::mat4 view)
{
    extern std::stack<int> color;

    if (colorCode == 16)
        shader.set("color", colorMap[color.top()].getValue());
    else
        shader.set("color", colorMap[colorCode].getValue());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
