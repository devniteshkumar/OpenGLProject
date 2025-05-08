#ifndef SM_HPP
#define SM_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// Load shader from file
std::string loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    if (file) {
        buffer << file.rdbuf();
    } else {
        std::cerr << "Failed to open shader file: " << filepath << "\n";
    }
    return buffer.str();
}

// Compile shader from file
unsigned int compileShader(const std::string& path, GLenum type) {
    std::string source = loadShaderSource(path);
    const char* src = source.c_str();

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Error checking
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error compiling " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
                  << " shader:\n" << infoLog << "\n";
    }

    return shader;
}


unsigned int loadShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    unsigned int vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Error linking shader program:\n" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

void reloadShaderProgram(unsigned int& program, const std::string& vertexPath, const std::string& fragmentPath) {
    glDeleteProgram(program);
    program = loadShaderProgram(vertexPath, fragmentPath);
    std::cout << "Shader program reloaded successfully.\n";
}


#endif // SM_HPP
