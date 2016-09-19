#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <GL.hpp>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <Debug.hpp>

class ShaderProgram {
public:
    ShaderProgram() = default;
    
    GLint getUniformLocation(const char* name) {
        return glGetUniformLocation(m_programID, name);
    };
    
    void start() {
        glUseProgram(m_programID);
    }
    
    void stop() {
        glUseProgram(0);
    }
    
    void cleanUp() {
        stop();
        glDetachShader(m_programID, m_vertexShaderID);
        glDetachShader(m_programID, m_fragmentShaderID);
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);
        glDeleteShader(m_programID);
    }
    
protected:
    void loadShaderFiles(const std::string& vertexFile, const std::string& fragmentFile) {
        m_vertexShaderID = loadShader(vertexFile, GL_VERTEX_SHADER);
        m_fragmentShaderID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
    }
    
    void initialize() {
        m_programID = glCreateProgram();
        glAttachShader(m_programID, m_vertexShaderID);
        glAttachShader(m_programID, m_fragmentShaderID);
        bindAttributes();
        glLinkProgram(m_programID);
        if (checkErrors(__LINE__, __FILE__)) exit(1);
        glValidateProgram(m_programID);
        
        getAllUniformLocations();
    }
    
    virtual void bindAttributes() { }
    
    virtual void getAllUniformLocations() { }
    
    void loadFloat(GLuint location, GLfloat value) {
        glUniform1f(location, value);
    }
    
    void loadVector3(GLuint location, const glm::vec3& vec3) {
        glUniform3f(location, vec3.x, vec3.y, vec3.z);
        if (checkErrors(__LINE__, __FILE__)) exit(1);
    }
    
    void loadUInt(GLuint location, GLuint value) {
        glUniform1ui(location, value);
    }
    
    void loadMatrix(GLuint location, const glm::mat4& mat4) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
    }
    
    void bindAttribute(GLuint attribute, const char* name) {
        glBindAttribLocation(m_programID, attribute, name);
    }
    
    GLuint loadShader(const std::string& file, int type) {
        auto stringContent = getFileContent(file);
        const char* content = stringContent.c_str();
        
        auto shaderID = glCreateShader(type);
        glShaderSource(shaderID, 1, &content, NULL);
        glCompileShader(shaderID);
        
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            GLchar log[512];
            glGetShaderInfoLog(shaderID, 512, NULL, &log[0]);
            std::cout << type << ":" << log << "\n";
        }
        
        return shaderID;
    }
    
    bool checkCompilation(GLint shaderID) {
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            GLchar log[512];
            glGetShaderInfoLog(shaderID, 512, NULL, &log[0]);
            std::cout << log << "\n";
            return false;
        }
        return true;
    }
    
    std::string getFileContent(const std::string& filename)
    {
        std::ifstream in(filename, std::ios::in | std::ios::binary);
        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
        }
        throw(errno);
    }
    
protected:
    int m_programID;
    int m_vertexShaderID;
    int m_fragmentShaderID;
};

#endif