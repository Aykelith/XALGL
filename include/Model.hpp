#ifndef MODEL_HPP
#define MODEL_HPP

#include <Global.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Mesh.hpp>
#include <Shader/ShaderProgram.hpp>

#include <SFML/Graphics/Image.hpp>
namespace Loader {
    static GLuint loadTexture(const std::string& file) {
        sf::Image texture;
        texture.loadFromFile(file);
        
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        return textureID;
    }
}

class Model {
public:
    Model() = default;
    Model(const std::string& path);

    // Draws the model, and thus all its meshes
    void draw(Shader::ShaderProgram& shader);
    
#ifdef EXPERIMENTAL_DRAW_SAME_MESH
    void drawE(Shader::ShaderProgram& shader, GLuint index);
    
    std::size_t meshesCount() { return m_meshes.size(); }
#endif
    
private:
    /*  Model Data  */
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_texturesLoaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    void loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName, Mesh& mesh);
};

#endif