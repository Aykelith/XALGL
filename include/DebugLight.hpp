#ifndef DEBUGLIGHT_HPP
#define DEBUGLIGHT_HPP

#include <GL.hpp>
#include <Debug.hpp>

class DebugLightModel {
public:
    DebugLightModel() {
        GLuint VBO;
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0); // Unbind m_VAO
    }
    
    GLuint getVAO() const { return m_VAO; }
    
private:
    GLuint m_VAO;
    
    GLfloat size = 0.3f;
    GLfloat vertices[108] = {
            -size, -size, -size,
             size, -size, -size,
             size,  size, -size,
             size,  size, -size,
            -size,  size, -size,
            -size, -size, -size,
    
            -size, -size,  size,
             size, -size,  size,
             size,  size,  size,
             size,  size,  size,
            -size,  size,  size,
            -size, -size,  size,
    
            -size,  size,  size,
            -size,  size, -size,
            -size, -size, -size,
            -size, -size, -size,
            -size, -size,  size,
            -size,  size,  size,
    
             size,  size,  size,
             size,  size, -size,
             size, -size, -size,
             size, -size, -size,
             size, -size,  size,
             size,  size,  size,
    
            -size, -size, -size,
             size, -size, -size,
             size, -size,  size,
             size, -size,  size,
            -size, -size,  size,
            -size, -size, -size,
    
            -size,  size, -size,
             size,  size, -size,
             size,  size,  size,
             size,  size,  size,
            -size,  size,  size,
            -size,  size, -size,
        };
};

static const char* VERTEX_SOURCE = "\
    #version 330 core\n \
    layout (location = 0) in vec3 position; \
    \
    uniform mat4 model; \
    uniform mat4 view; \
    uniform mat4 projection; \
    \
    void main() \
    { \
        gl_Position = projection * view * model * vec4(position, 1.f); \
    }";
    
static const char* FRAGMENT_SOURCE = "\
    #version 330 core\n \
    \
    out vec4 outColor; \
    \
    uniform vec3 color; \
    \
    void main() \
    { \
        outColor = vec4(color, 1.f); \
    }";


class DebugLightShader: public ShaderProgram {
public:
    DebugLightShader(const glm::mat4& perspectiveMatrix) {
        m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_vertexShaderID, 1, &VERTEX_SOURCE, NULL);
        glCompileShader(m_vertexShaderID);
        assert(checkCompilation(m_vertexShaderID));
        
        m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_fragmentShaderID, 1, &FRAGMENT_SOURCE, NULL);
        glCompileShader(m_fragmentShaderID);
        assert(checkCompilation(m_fragmentShaderID));
        
        initialize();
        
        start();
        loadProjectionMatrix(perspectiveMatrix);
        stop();
    }
    
    void loadModelMatrix(const glm::mat4& mat4) {
        loadMatrix(m_locationModelMatrix, mat4);
    }
    
    void loadProjectionMatrix(const glm::mat4& mat4) {
        loadMatrix(m_locationProjectionMatrix, mat4);
    }
    
    void loadViewMatrix(Camera &camera) {
        loadMatrix(m_locationViewMatrix, camera.getViewMatrix());
    }
    
    void loadColor(const glm::vec3& color) {
        loadVector3(m_locationColor, color);
    }
    
protected:
    void bindAttributes() {
        bindAttribute(0, "position");
    }
    
    void getAllUniformLocations() {
        m_locationModelMatrix      = getUniformLocation("model");
        m_locationProjectionMatrix = getUniformLocation("projection");
        m_locationViewMatrix       = getUniformLocation("view");
        m_locationColor            = getUniformLocation("color");
        
        assert(m_locationModelMatrix != -1);
        assert(m_locationProjectionMatrix != -1);
        assert(m_locationViewMatrix != -1);
    }
    
protected:
    GLint m_locationModelMatrix;
    GLint m_locationProjectionMatrix;
    GLint m_locationViewMatrix;
    GLint m_locationColor;
};

#endif