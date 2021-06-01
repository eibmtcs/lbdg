#ifndef __RENDER_HPP__
#define __RENDER_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "camera.hpp"
#include "shader.hpp"
#include "subFile.hpp"

class Render
{
public:
    Render();
    int show(std::vector<std::shared_ptr<Model>> model, std::string name);
    int makePNG(std::vector<std::shared_ptr<Model>> model, std::string name);
    int setShader(std::string vertexPath, std::string fragmentPath);
    int setVBO(std::vector<float> buffer);
    int setVAP(int location, int size, int stride, int offset);
    int setEBO(std::vector<unsigned int> buffer);
    int createTexture(std::string image, GLint format, std::string uniform, int textureUnits);
    int lineMode();
    int fillMode();
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void processInput(GLFWwindow* window);
    std::vector<std::shared_ptr<Model>> a;
    std::string name;

private:
    GLFWwindow *window;
    unsigned int VAO, VBO, EBO;
    Shader shader;
    std::vector<unsigned int> texture;

    static Camera camera;
    Camera tmp;
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;
    static bool firstMouse;
    static float lastX, lastY;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

#endif
