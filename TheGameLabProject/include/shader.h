
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath) {

        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexShaderCodeString;
        std::string fragmentShaderCodeString;
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;
        // ensure ifstream objects can throw exceptions:
        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open the files
            vertexShaderFile.open(vertexPath);
            fragmentShaderFile.open(fragmentPath);
            // read file's buffer contents into streams
            std::stringstream vertexShaderStream;
            std::stringstream  fragmentShaderStream;
            vertexShaderStream  << vertexShaderFile.rdbuf();
            fragmentShaderStream  << fragmentShaderFile.rdbuf();
            // close file handlers
            vertexShaderFile.close();
            fragmentShaderFile.close();
            // convert stream into strings
            vertexShaderCodeString = vertexShaderStream.str();
            fragmentShaderCodeString = fragmentShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char* vertexShaderCode = vertexShaderCodeString.c_str();
        const char* fragmentShaderCode = fragmentShaderCodeString.c_str();

        // 2. Compile the Shaders
        int success;
        char infoLog[512];

        // Vertex Shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShader);
        // Check and print compile errors if any
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Fragment Shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);
        // Check and print compile errors if any
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        // Check and print linking errors if any
        glGetShaderiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // use/activate the shader
    void use() const {
        glUseProgram(ID);
    }

    // Utility uniform value setting functions
    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};

#endif