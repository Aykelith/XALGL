#ifndef DEBUG_LIGHT_HPP
#define DEBUG_LIGHT_HPP

#include <Global.hpp>

#include <GL.hpp>
#include <Debug.hpp>
#include <Shader/ShaderProgram.hpp>

namespace Debug {    
    class LightModel {
    public:
        LightModel() {
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
            -size, -size, -size, // Bottom-left
             size,  size, -size, // top-right
             size, -size, -size, // bottom-right         
             size,  size, -size, // top-right
            -size, -size, -size, // bottom-left
            -size,  size, -size, // top-left
            // Front face
            -size, -size,  size, // bottom-left
             size, -size,  size, // bottom-right
             size,  size,  size, // top-right
             size,  size,  size, // top-right
            -size,  size,  size, // top-left
            -size, -size,  size, // bottom-left
            // Left face
            -size,  size,  size, // top-right
            -size,  size, -size, // top-left
            -size, -size, -size, // bottom-left
            -size, -size, -size, // bottom-left
            -size, -size,  size, // bottom-right
            -size,  size,  size, // top-right
            // Right face
             size,  size,  size, // top-left
             size, -size, -size, // bottom-right
             size,  size, -size, // top-right         
             size, -size, -size, // bottom-right
             size,  size,  size, // top-left
             size, -size,  size, // bottom-left     
            // Bottom face
            -size, -size, -size, // top-right
             size, -size, -size, // top-left
             size, -size,  size, // bottom-left
             size, -size,  size, // bottom-left
            -size, -size,  size, // bottom-right
            -size, -size, -size, // top-right
            // Top face
            -size,  size, -size, // top-left
             size,  size,  size, // bottom-right
             size,  size, -size, // top-right     
             size,  size,  size, // bottom-right
            -size,  size, -size, // top-left
            -size,  size,  size  // bottom-left        
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


    class LightShader: public Shader::ShaderProgram {
    public:
        static const uint UNIFORM_COLOR = Shader::Uniform::_COUNT + 1;
        
    public:
        LightShader(const glm::mat4& perspectiveMatrix) {
            loadShader(VERTEX_SOURCE, GL_VERTEX_SHADER);
            loadShader(FRAGMENT_SOURCE, GL_FRAGMENT_SHADER);
            createProgram();
            bindAttribute(0, "position");
            link();
            storeUniformLocation(Shader::Uniform::MODEL_MATRIX, "model");
            storeUniformLocation(Shader::Uniform::VIEW_MATRIX, "view");
            storeUniformLocation(Shader::Uniform::PROJECTION_MATRIX, "projection");
            storeUniformLocation(UNIFORM_COLOR, "color");
            start();
            loadMatrix(Shader::Uniform::PROJECTION_MATRIX, perspectiveMatrix);
            stop();
        }
    };
}

#endif