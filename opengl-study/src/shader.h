#ifndef SHADER_H
#define SHADER_H
#include "../include/glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.h"

class Shader 
{
public:
    // shader programer id
    unsigned int id;
    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode, fragmentCode;
        std::ifstream vShaderFile, fShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode  = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e) {
            std::cout << "fail to read shader file" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        
        std::cout << "vertex shader compile ret:" << success << std::endl;
        if(!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "vertex shader compile failed info:" << infoLog << std::endl;
        }

        // 片段着色器
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        
        std::cout << "fragment shader compile ret:" << success << std::endl;
        if(!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "fragment shader compile failed info:" << infoLog << std::endl;
        }

        // 将着色器对象链接到一个用于渲染的着色器程序对象(shader program object)中

        // 上一个着色器的输入会链接到下一个着色器的输入，当输入输出不匹配时会链接错误
        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);

        glGetProgramiv(id, GL_LINK_STATUS, &success);
        std::cout << "shader program link ret:" << success << std::endl;
        if(!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "shader program link failed info:" << infoLog << std::endl;
        }


        // 着色器链接到 shader program object 后, 删除 shader 对象
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    }

    void use() {
        glUseProgram(id);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void setMat4f(const std::string &name, float* value) const {
        int modelLoc = glGetUniformLocation(id, name.c_str());
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value);
    }

};
#endif