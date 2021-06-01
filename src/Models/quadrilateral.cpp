#include <memory>
#include <stack>

#include "model.hpp"
#include "subFile.hpp"
#include "quadrilateral.hpp"
#include "point.hpp"

Quadrilateral::Quadrilateral(Point a, Point b, Point c, Point d)
{
    type = QUADRILATERAL;
    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    Point vertices[] = { a, b, c, d};


    unsigned int indices[] = { 0, 1, 3, 1, 2, 3};
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));

    glEnableVertexAttribArray(0);
}

Quadrilateral::Quadrilateral(int color, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4)
  : Quadrilateral(Point(x1, y1, z1), Point(x2, y2, z2), Point(x3, y3, z3), Point(x4, y4, z4))
{
      colorCode = color;
}

Quadrilateral::Quadrilateral(int color, std::string s)
{
    float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    std::istringstream read(s);
    read >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3 >> x4 >> y4 >> z4;
    new(this) Quadrilateral(color, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);
}

void Quadrilateral::show(Shader shader, glm::mat4 projection, glm::mat4 view)
{
    extern std::stack<int> color;

    if (colorCode == 16)
        shader.set("color", colorMap[color.top()].getValue());
    else
        shader.set("color", colorMap[colorCode].getValue());

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
