#include <iostream>
#include <functional>

#include "function.hpp"
#include "model.hpp"
#include "optionalLine.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stack>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "camera.hpp"

#include "subFile.hpp"

#include "render.hpp"
//glm::vec3(300, -200, 300)
Camera Render::camera(glm::vec3(300, -200, 300), glm::vec3(0, -76, 0));
unsigned int Render::SCR_WIDTH = 800;
unsigned int Render::SCR_HEIGHT = 600;
bool Render::firstMouse = true;
float Render::lastX = Render::SCR_WIDTH / 2;
float Render::lastY = Render::SCR_HEIGHT / 2;

Render::Render()
{
    glfwInit();
    //glfw初始化和配置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_SAMPLES, 4);

    /* glfwWindowHint(GLFW_DEPTH_BITS, 32); */

    //glfw创建一个窗口
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW 3.3", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window " << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);


    //窗口大小调整时回调函数
    //glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)&Render::framebuffer_size_callback);

    //glad载入所有OpengGl函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    //开启OpenGL深度测试
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);
    
    /* glEnable(GL_CULL_FACE); */

    //开启透明度
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //设置画线偏移
    glPolygonOffset(-2.0f, -2.0f);
    glEnable(GL_POLYGON_OFFSET_LINE);

    //设置线宽
    glLineWidth(1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, (GLFWcursorposfun)&Render::mouse_callback);

    camera.MovementSpeed = 70;
    /* glfwDestroyWindow(window); */
    /* glfwTerminate(); */
}

int Render::show(std::vector<std::shared_ptr<Model>> model, std::string name)
{

    glfwSetWindowTitle(window, name.c_str());
    this->name = name;

    Render::camera = Camera(Render::camera.Position, glm::vec3(0, -76, 0));
    glfwShowWindow(window);

    extern std::map<std::string, std::pair<std::vector<glm::mat4>, std::pair<std::vector<glm::vec4>, std::vector<glm::vec4>>>> indeces;
    extern std::map<std::string, std::shared_ptr<Subfile>> partMap;

    unsigned int buffer, buffer2;
    glGenBuffers(1, &buffer);
    glGenBuffers(1, &buffer2);
    bool fristrun = true;
    //渲染循环
    while (glfwGetWindowAttrib(window, GLFW_VISIBLE)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //处理输入
        processInput(window);

        /* glViewport(0, 0, 400, 300); */
        glViewport(0, 0, 800, 600);
        //设置清空屏幕用的颜色
        glClearColor(230.0f * (1.0f/255.0f), 230.0f * (1.0f/255.0f), 230.0f * (1.0f/255.0f), 1.0f);
        //清空颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(Render::camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 5.0f, 1000.0f);
        glm::mat4 view       = Render::camera.GetViewMatrix();

        shader.set("projection", projection);
        shader.set("view", view);

        auto map_it = indeces.begin();
        while (map_it != indeces.end()) {
            if (partMap.find(map_it->first) != partMap.end() && !partMap[map_it->first]->modelTree.empty()) {
                for (auto &i : partMap[map_it->first]->modelTree) {

                    glBindBuffer(GL_ARRAY_BUFFER, buffer2);
                    if (i->type == LINE || i->type == OPTIONALLINE)
                        glBufferData(GL_ARRAY_BUFFER, map_it->second.first.size() * sizeof(glm::vec4), &(map_it->second.second.first)[0], GL_STATIC_DRAW);
                    else
                        glBufferData(GL_ARRAY_BUFFER, map_it->second.first.size() * sizeof(glm::vec4), &(map_it->second.second.second)[0], GL_STATIC_DRAW);

                    glBindBuffer(GL_ARRAY_BUFFER, buffer);
                    glBufferData(GL_ARRAY_BUFFER, map_it->second.first.size() * sizeof(glm::mat4), &(map_it->second.first)[0], GL_STATIC_DRAW);
                    if (fristrun) {
                        GLsizei vec4Size = sizeof(glm::vec4);
                        glBindBuffer(GL_ARRAY_BUFFER, buffer);
                        glEnableVertexAttribArray(3);
                        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(0 * vec4Size));
                        glEnableVertexAttribArray(4);
                        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(1 * vec4Size));
                        glEnableVertexAttribArray(5);
                        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(2 * vec4Size));
                        glEnableVertexAttribArray(6);
                        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(3 * vec4Size));
                        glVertexAttribDivisor(3, 1);
                        glVertexAttribDivisor(4, 1);
                        glVertexAttribDivisor(5, 1);
                        glVertexAttribDivisor(6, 1);
                        glEnableVertexAttribArray(0);

                        glBindBuffer(GL_ARRAY_BUFFER, buffer2);
                        glEnableVertexAttribArray(1);
                        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vec4Size, reinterpret_cast<void*>(0 * vec4Size));
                        glVertexAttribDivisor(1, 1);
                        glEnableVertexAttribArray(0);
                    }

                    if (i->type != SUBFILE) {
                        glBindVertexArray(i->VAO);
                        if (i->type == LINE) {
                            glDrawArraysInstanced(GL_LINES, 0, 2, map_it->second.first.size());
                        } else if (i->type == OPTIONALLINE) {
                            int a = 3;
                            if (a == 1) {
                                std::vector<glm::mat4> modelcopy(map_it->second.first);
                                std::vector<glm::vec4> colorcopy(map_it->second.second.first);
                                auto j = modelcopy.begin();
                                auto k = colorcopy.begin();
                                while (j != modelcopy.end() && k != colorcopy.end()) {
                                    glm::vec4 a1 = projection * view * (*j) * glm::vec4((glm::vec3)i->vertices[0], 1.0);
                                    glm::vec4 b1 = projection * view * (*j) * glm::vec4((glm::vec3)i->vertices[1], 1.0);
                                    glm::vec4 c1 = projection * view * (*j) * glm::vec4((glm::vec3)i->vertices[2], 1.0);
                                    glm::vec4 d1 = projection * view * (*j) * glm::vec4((glm::vec3)i->vertices[3], 1.0);
                                    glm::vec3 a = glm::vec3(a1.x/a1.w, a1.y/a1.w, a1.z/a1.w);
                                    glm::vec3 b = glm::vec3(b1.x/b1.w, b1.y/b1.w, b1.z/b1.w);
                                    glm::vec3 c = glm::vec3(c1.x/c1.w, c1.y/c1.w, c1.z/c1.w);
                                    glm::vec3 d = glm::vec3(d1.x/d1.w, d1.y/d1.w, d1.z/d1.w);
                                    if (((c.x - a.x) / (b.x - a.x) - (c.y - a.y) / (b.y - a.y)) > 0
                                            == ((d.x - a.x) / (b.x - a.x) - (d.y - a.y) / (b.y - a.y)) > 0) {
                                        j++; k++;
                                    } else {
                                        j = modelcopy.erase(j); k = colorcopy.erase(k);
                                    }
                                }
                                glBufferData(GL_ARRAY_BUFFER, modelcopy.size() * sizeof(glm::mat4), &(modelcopy)[0], GL_STATIC_DRAW);
                                glBindBuffer(GL_ARRAY_BUFFER, buffer2);
                                glBufferData(GL_ARRAY_BUFFER, colorcopy.size() * sizeof(glm::vec4), &(colorcopy)[0], GL_STATIC_DRAW);
                                glDrawArraysInstanced(GL_LINES, 0, 2, modelcopy.size());
                            } else if (a == 2) {
                                glDrawArraysInstanced(GL_LINES, 0, 2, map_it->second.first.size());
                            } else if (a == 3) {
                            }
                        } else if (i->type == TRIANGLE) {
                            glDrawArraysInstanced(GL_TRIANGLES, 0, 3, map_it->second.first.size());
                        } else if (i->type == QUADRILATERAL) {
                            glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, map_it->second.first.size());
                        }
                    }
                }
            }
            ++map_it;
        }
        fristrun = false;

        /* for (auto &i : model) */
            /* i->show(shader, projection, view); */

        /* std::cout */ 
        /*     << "Position: " << camera.Position.x << "   " << camera.Position.y << "   " << camera.Position.z << "\n" */
        /*     << "Up:       " << camera.Up.x << "   " << camera.Up.y << "   " << camera.Up.z << "\n" */
        /*     << "Yaw:      " << camera.Yaw << "\n" */
        /*     << "Pitch:    " << camera.Pitch << "\n" */
        /*     << std::endl; */

        /* glLoadIdentity(); */
        /* //glOrtho(; */
        /* glMatrixMode(GL_PROJECTION); */
        /* glDisable(GL_DEPTH_TEST); */
        /* glLoadIdentity(); */
        /* glViewport(400, 0, 400, 300); */
        /* glMatrixMode(GL_MODELVIEW); */
        /* glBegin(GL_POLYGON); */
        /* glVertex2f(-0.5, -0.5); */
        /* glVertex2f(-0.5, 0.5); */
        /* glVertex2f(0.5, 0.5); */
        /* glVertex2f(0.5, -0.5); */
        /* glEnd(); */
        /* glFlush(); */

        //glClearColor(0.0f * (1.0f/255.0f), 0.0f * (1.0f/255.0f), 0.0f * (1.0f/255.0f), 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //交换缓冲区和轮询IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* glfwDestroyWindow(window); */

    //释放所有资源
    /* glfwTerminate(); */

    return 0;
}

int Render::makePNG(std::vector<std::shared_ptr<Model>> model, std::string name)
{
    glfwSetWindowTitle(window, name.c_str());
    glfwHideWindow(window);
    this->name = name;

    Render::camera = Camera(Render::camera.Position, glm::vec3(0, -76, 0));

    extern std::map<std::string, std::pair<std::vector<glm::mat4>, std::pair<std::vector<glm::vec4>, std::vector<glm::vec4>>>> indeces;
    extern std::map<std::string, std::shared_ptr<Subfile>> partMap;

    unsigned int buffer, buffer2;
    glGenBuffers(1, &buffer);
    glGenBuffers(1, &buffer2);
    bool fristrun = true;

    for (int i = 0; i < 3; ++i) {

    glViewport(0, 0, 800, 600);
    //设置清空屏幕用的颜色
    glClearColor(230.0f * (1.0f/255.0f), 230.0f * (1.0f/255.0f), 230.0f * (1.0f/255.0f), 1.0f);
    //清空颜色缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(Render::camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 5.0f, 1000.0f);
    glm::mat4 view       = Render::camera.GetViewMatrix();

    shader.set("projection", projection);
    shader.set("view", view);

    auto map_it = indeces.begin();
    while (map_it != indeces.end()) {
        if (partMap.find(map_it->first) != partMap.end() && !partMap[map_it->first]->modelTree.empty()) {
            for (auto &i : partMap[map_it->first]->modelTree) {

                glBindBuffer(GL_ARRAY_BUFFER, buffer2);
                if (i->type == LINE || i->type == OPTIONALLINE)
                    glBufferData(GL_ARRAY_BUFFER, map_it->second.first.size() * sizeof(glm::vec4), &(map_it->second.second.first)[0], GL_STATIC_DRAW);
                else
                    glBufferData(GL_ARRAY_BUFFER, map_it->second.first.size() * sizeof(glm::vec4), &(map_it->second.second.second)[0], GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                glBufferData(GL_ARRAY_BUFFER, map_it->second.first.size() * sizeof(glm::mat4), &(map_it->second.first)[0], GL_STATIC_DRAW);
                if (fristrun) {
                    GLsizei vec4Size = sizeof(glm::vec4);
                    glBindBuffer(GL_ARRAY_BUFFER, buffer);
                    glEnableVertexAttribArray(3);
                    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(0 * vec4Size));
                    glEnableVertexAttribArray(4);
                    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(1 * vec4Size));
                    glEnableVertexAttribArray(5);
                    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(2 * vec4Size));
                    glEnableVertexAttribArray(6);
                    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(3 * vec4Size));
                    glVertexAttribDivisor(3, 1);
                    glVertexAttribDivisor(4, 1);
                    glVertexAttribDivisor(5, 1);
                    glVertexAttribDivisor(6, 1);
                    glEnableVertexAttribArray(0);

                    glBindBuffer(GL_ARRAY_BUFFER, buffer2);
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vec4Size, reinterpret_cast<void*>(0 * vec4Size));
                    glVertexAttribDivisor(1, 1);
                    glEnableVertexAttribArray(0);
                }

                if (i->type != SUBFILE) {
                    glBindVertexArray(i->VAO);
                    if (i->type == LINE) {
                        glDrawArraysInstanced(GL_LINES, 0, 2, map_it->second.first.size());
                    } else if (i->type == OPTIONALLINE) {
                        int a = 1;
                        if (a == 1) {
                            std::vector<glm::mat4> modelcopy(map_it->second.first);
                            std::vector<glm::vec4> colorcopy(map_it->second.second.first);
                            auto j = modelcopy.begin();
                            auto k = colorcopy.begin();
                            while (j != modelcopy.end() && k != colorcopy.end()) {
                                glm::vec4 a1 = projection * view * (*j) * glm::vec4((glm::vec3)i->vertices[0], 1.0);
                                glm::vec4 b1 = projection * view * (*j) * glm::vec4((glm::vec3)i->vertices[1], 1.0);
                                glm::vec4 c1 = projection * view * (*j) * glm::vec4((glm::vec3)i->vertices[2], 1.0);
                                glm::vec4 d1 = projection * view * (*j) * glm::vec4((glm::vec3)i->vertices[3], 1.0);
                                glm::vec3 a = glm::vec3(a1.x/a1.w, a1.y/a1.w, a1.z/a1.w);
                                glm::vec3 b = glm::vec3(b1.x/b1.w, b1.y/b1.w, b1.z/b1.w);
                                glm::vec3 c = glm::vec3(c1.x/c1.w, c1.y/c1.w, c1.z/c1.w);
                                glm::vec3 d = glm::vec3(d1.x/d1.w, d1.y/d1.w, d1.z/d1.w);
                                if (((c.x - a.x) / (b.x - a.x) - (c.y - a.y) / (b.y - a.y)) > 0
                                        == ((d.x - a.x) / (b.x - a.x) - (d.y - a.y) / (b.y - a.y)) > 0) {
                                    j++; k++;
                                } else {
                                    j = modelcopy.erase(j); k = colorcopy.erase(k);
                                }
                            }
                            glBufferData(GL_ARRAY_BUFFER, modelcopy.size() * sizeof(glm::mat4), &(modelcopy)[0], GL_STATIC_DRAW);
                            glBindBuffer(GL_ARRAY_BUFFER, buffer2);
                            glBufferData(GL_ARRAY_BUFFER, colorcopy.size() * sizeof(glm::vec4), &(colorcopy)[0], GL_STATIC_DRAW);
                            glDrawArraysInstanced(GL_LINES, 0, 2, modelcopy.size());
                        } else if (a == 2) {
                            glDrawArraysInstanced(GL_LINES, 0, 2, map_it->second.first.size());
                        } else if (a == 3) {
                        }
                    } else if (i->type == TRIANGLE) {
                        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, map_it->second.first.size());
                    } else if (i->type == QUADRILATERAL) {
                        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, map_it->second.first.size());
                    }
                }
            }
        }
        ++map_it;
    }
    fristrun = false;
    }

    extern std::string modelName;
    saveRender2Png(SCR_WIDTH, SCR_HEIGHT, "./" + modelName + "/images/" + name + ".png");

    //释放所有资源
    /* glfwTerminate(); */

    return 0;
}

//创建一个纹理
int Render::createTexture(std::string image, GLint format, std::string uniform, int textureUnits)
{
    unsigned int texture1;
    //创建纹理ID
    glGenTextures(1, &texture1);
    texture.push_back(texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //指定纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //指定纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //加载并生成纹理
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(image.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        //创建多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
        std::cerr << "Failed to load texture" << std::endl;
    stbi_image_free(data);
    shader.use();
    shader.set(uniform.c_str(), textureUnits);
    return 0;
}

//将顶点数组复制到OpenGl中使用
int Render::setVBO(std::vector<float> buffer)
{
    //vertices = buffer;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), &buffer[0], GL_STATIC_DRAW);
    return 0;
}
//设置顶点属性指针
int Render::setVAP(int location, int size, int stride, int offset)
{
    glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(location);
    return 0;
}

int Render::setEBO(std::vector<unsigned int> buffer)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * buffer.size(), &buffer[0], GL_STATIC_DRAW);
    return 0;
}

int Render::lineMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    return 0;
}

int Render::fillMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    return 0;
}

int Render::setShader(std::string vertexPath, std::string fragmentPath)
{
    shader = Shader(vertexPath.c_str(), fragmentPath.c_str());
    return 0;
}

void Render::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Render::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    Render::camera.ProcessMouseMovement(xoffset, yoffset);
}

void Render::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwHideWindow(window);
        extern std::string modelName;
        saveRender2Png(SCR_WIDTH, SCR_HEIGHT, "./" + modelName + "/images/" + name + ".png");
        firstMouse = true;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Render::camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Render::camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Render::camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Render::camera.ProcessKeyboard(RIGHT, deltaTime);
}
