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
#include <Loader.hpp>
#include <StaticShader.hpp>

class Model {
public:
    Model() = default;
    Model(const std::string& path);

    // Draws the model, and thus all its meshes
    void draw(StaticShader& shader);
    
#ifdef EXPERIMENTAL_DRAW_SAME_MESH
    void drawE(StaticShader& shader, GLuint index);
    
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