#ifndef MESH_HPP
#define MESH_HPP

#include <Global.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/types.h>

#include <Shader/ShaderProgram.hpp>

struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
};

enum TextureType {
    Diffuse = 0,
    Specular
};

inline std::string getTextureTypeName(TextureType type) {
    switch (type) {
        case TextureType::Diffuse: return "texture_diffuse";
        case TextureType::Specular: return "texture_specular";
        default: return "";
    }
}

struct Texture {
    GLuint id;
    TextureType type;
    aiString path;
};

class Mesh {
public:
    Mesh() = default;
    
    void setVerticesCount(uint size) { m_vertices.reserve(size); }
    void setTexturesCount(uint size) { m_textures.reserve(size); }
    void setIndicesCount(uint size) { m_indices.reserve(size); }
    
    void addVertex(const Vertex& vertex) { m_vertices.push_back(vertex); }
    void addTexture(const Texture& texture) { m_textures.push_back(texture); }
    void addIndex(GLuint index) { m_indices.push_back(index); }

    // Render the mesh
    void draw(Shader::ShaderProgram& shader);

    // Initializes all the buffer objects/arrays
    void glue();

#ifdef MODEL_CONSOLE_MESH_INFO
    std::string d_info;
#endif
    
#ifdef EXPERIMENTAL_DRAW_SAME_MESH
    void bindTextures(StaticShader& shader);
    void unbindTextures();
    void drawE(StaticShader& shader);
#endif
    
protected:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Texture> m_textures;
};

#endif
