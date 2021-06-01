#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <glad/glad.h>
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
        unsigned int ID;

        Shader() = default;
        Shader(const char* vertexPath, const char* fragmentPath);

        void use();

        void set(const std::string &name, bool value) const;
        void set(const std::string &name, int value) const;
        void set(const std::string &name, float value) const;
        void set(const std::string &name, const glm::mat4 &value) const;
        void set(const std::string &name, const glm::vec3 &value) const;
        void set(const std::string &name, const glm::vec4 &value) const;
};

#endif
