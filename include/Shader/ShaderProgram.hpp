#ifndef SHADER_SHADERPROGRAM_HPP
#define SHADER_SHADERPROGRAM_HPP

#include <Global.hpp>

#include <GL.hpp>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Debug.hpp>
#include <Shader/Uniforms.hpp>

#include <unordered_map>

namespace Shader {
    enum class Uniform : int {
        MODEL_MATRIX = Impl::BASE_START,
        VIEW_MATRIX,
        PROJECTION_MATRIX,
        VIEW_POS,
        MATERIAL,
        MATERIAL_SHININESS = MATERIAL + Impl::MAX_MATERIALS,
        MATERIAL_DIFFUSE_TEXTURE,
        MATERIAL_SPECULAR_TEXTURE = MATERIAL_DIFFUSE_TEXTURE + Impl::MAX_MATERIAL_DIFFUSE_TEXTURES,
        _COUNT
    };
    static_assert(static_cast<int>(Uniform::_COUNT) == Impl::BASE_START + Impl::BASE_COUNT, "");

    class ShaderProgram {
    public:
        ShaderProgram() = default;

        GLuint getProgramID() const { return m_programID; }

        void loadShader(const char* data, int type) {
            auto shaderID = glCreateShader(type);
            glShaderSource(shaderID, 1, &data, NULL);
            glCompileShader(shaderID);

            GLint success;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE) {
                GLchar log[512];
                glGetShaderInfoLog(shaderID, 512, NULL, &log[0]);
                std::cout << ((type == GL_VERTEX_SHADER) ? "VERT" : "FRAG") << ":" << log << "\n";
            }

            if (type == GL_VERTEX_SHADER) {
                m_vertexShaderID = shaderID;
            } else {
                m_fragmentShaderID = shaderID;
            }
        }

        void loadShader(const std::string& file, int type) {
            auto stringContent = getFileContent(file);
            std::cout << "0\n";
            loadShader(stringContent.c_str(), type);
        }

        void loadShader(const std::string& vertFile, const std::string& fragFile) {
            loadShader(vertFile, GL_VERTEX_SHADER);
            loadShader(fragFile, GL_FRAGMENT_SHADER);
        }

        void createProgram() {
            m_programID = glCreateProgram();
            glAttachShader(m_programID, m_vertexShaderID);
            glAttachShader(m_programID, m_fragmentShaderID);
        }

        void bindAttribute(GLuint attribute, const char* name) { glBindAttribLocation(m_programID, attribute, name); }

        void link() {
            glLinkProgram(m_programID);
            if (checkErrors(__LINE__, __FILE__)) exit(1);
            glValidateProgram(m_programID);
        }

        void loadInt(uint id, GLint value) { glUniform1i(m_uniforms[id], value); assert(!checkErrors()); }
        void loadUInt(uint id, GLuint value) { glUniform1ui(m_uniforms[id], value); assert(!checkErrors()); }
        void loadFloat(uint id, GLfloat value) { glUniform1f(m_uniforms[id], value);  }
        void loadVector3(uint id, const glm::vec3& vec3) { glUniform3f(m_uniforms[id], vec3.x, vec3.y, vec3.z); assert(!checkErrors()); }
        void loadMatrix(uint id, const glm::mat4& mat4) { glUniformMatrix4fv(m_uniforms[id], 1, GL_FALSE, glm::value_ptr(mat4)); assert(!checkErrors()); }

        void storeUniformLocation(uint id, const char* name) {
            m_uniforms.emplace(id, glGetUniformLocation(m_programID, name));
            assert(!checkErrors());
            assert(m_uniforms[id] != -1);
        }

        GLint getUniformLocation(const char* name) const {
            return glGetUniformLocation(m_programID, name);
        }

        void start() { glUseProgram(m_programID); }
        void stop() { glUseProgram(0); }

        void cleanUp() {
            stop();
            glDetachShader(m_programID, m_vertexShaderID);
            glDetachShader(m_programID, m_fragmentShaderID);
            glDeleteShader(m_vertexShaderID);
            glDeleteShader(m_fragmentShaderID);
            glDeleteShader(m_programID);
        }

    protected:
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

        std::string getFileContent(const std::string& filename) {
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
        GLuint m_programID;
        GLuint m_vertexShaderID;
        GLuint m_fragmentShaderID;

        std::unordered_map<uint, GLint> m_uniforms;
    };
}

#endif
