#ifndef STATICSHADER_HPP
#define STATICSHADER_HPP

#include <ShaderProgram.hpp>
#include <glm/glm.hpp>
#include <Camera.hpp>

#include <Debug.hpp>

class StaticShader: public ShaderProgram {
private:
    static constexpr const char* VERTEX_FILE = "res/vertex.shader";
    static constexpr const char* FRAGMENT_FILE = "res/fragment.shader";

public:
    StaticShader(const glm::mat4& perspectiveMatrix) {
        loadShaderFiles(VERTEX_FILE, FRAGMENT_FILE);
        initialize();
        Program = m_programID;
        
        start();
        loadProjectionMatrix(perspectiveMatrix);
        stop();
    }
    
    GLuint Program;
    
    void loadModelMatrix(const glm::mat4& mat4) {
        loadMatrix(m_locationModelMatrix, mat4);
    }
    
    void loadProjectionMatrix(const glm::mat4& mat4) {
        loadMatrix(m_locationProjectionMatrix, mat4);
    }
    
    void loadViewMatrix(Camera &camera) {
        loadMatrix(m_locationViewMatrix, camera.getViewMatrix());
    }
    
    void loadViewPos(const glm::vec3& pos) {
        loadVector3(m_locationViewPos, pos);
    }
    
protected:
    void bindAttributes() {
        bindAttribute(0, "position");
        bindAttribute(1, "normal");
        bindAttribute(2, "texCoords");
    }
    
    void getAllUniformLocations() {
        m_locationModelMatrix      = getUniformLocation("model");
        m_locationProjectionMatrix = getUniformLocation("projection");
        m_locationViewMatrix       = getUniformLocation("view");
        m_locationViewPos          = getUniformLocation("viewPos");
        
        assert(m_locationModelMatrix != -1);
        assert(m_locationProjectionMatrix != -1);
        assert(m_locationViewMatrix != -1);
    }
    
protected:
    GLint m_locationModelMatrix;
    GLint m_locationProjectionMatrix;
    GLint m_locationViewMatrix;
    GLint m_locationViewPos;
};

#endif