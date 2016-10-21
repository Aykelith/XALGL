#include <Model.hpp>

#ifdef MODEL_CONSOLE_INFO
#include <iostream>

uint d_totalMeshes = 0;

    #ifdef MODEL_CONSOLE_TEX_INFO
        std::string d_texInfo;
        uint        d_texCount = 0;
    #endif
    
#include <functional>
#endif

Model::Model(const std::string& path) {
    // Read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // Check for errors
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // Retrieve the directory path of the filepath
    m_directory = path.substr(0, path.find_last_of('/'));

    // Process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
    
#ifdef MODEL_CONSOLE_INFO
    std::cout << "= MODEL ===========================\n"
              << "Model file: " << path << "\n"
              << "Meshes: " << d_totalMeshes << "\n";
              
    #ifdef MODEL_CONSOLE_MESH_INFO
        for (std::size_t i=0; i<m_meshes.size(); ++i) {
            std::cout << i+1 << "." << m_meshes[i].d_info;
        }
    #endif
      
    #ifdef MODEL_CONSOLE_TEX_INFO
        std::cout << "Textures: " << m_texturesLoaded.size() << "\n"
                  << ((m_texturesLoaded.size() > 0) ? d_texInfo : "");
    #endif
    
    #ifdef MODEL_CONSOLE_MORE_INFO
        std::cout << "Has:\n"
                  << "\tAnimations: " << scene->mNumAnimations << "\n"
                  << "\tCameras: " <<  scene->mNumCameras << "\n"
                  << "\tLights: " <<  scene->mNumLights << "\n"
                  << "\tMaterials: " <<  scene->mNumMaterials << "\n"
                  << "\tMeshes: " <<  scene->mNumMeshes << "\n"
                  << "\tTextures: " <<  scene->mNumCameras << "\n";
    #endif
    
    std::cout << "===================================\n";
#endif
}

void Model::draw(Shader::ShaderProgram& shader) {
    for(GLuint i = 0; i < m_meshes.size(); i++)
        m_meshes[i].draw(shader);
}

#ifdef EXPERIMENTAL_DRAW_SAME_MESH
void Model::drawE(Shader::ShaderProgram& shader, GLuint index) {
    m_meshes[index].draw(shader);
}
#endif

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process each mesh located at the current node
    // uint m_meshesCount = 0;
    // for(GLuint i = 0; i < node->mNumMeshes; i++) {
    //     m_meshesCount += node->mNumMeshes;
    // }
    // m_meshes.reserve(m_meshesCount);
    
#ifdef MODEL_CONSOLE_INFO
    d_totalMeshes += node->mNumMeshes;
#endif
    
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        // The node object only contains indices to index the actual objects in the scene. 
        // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
        m_meshes.push_back(processMesh(mesh, scene));			
    }
    // After we've processed all of the m_meshes (if any) we then recursively process each of the children nodes
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh meshObject;
    meshObject.setVerticesCount(mesh->mNumVertices);
    
    uint indicesCount = 0;
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        indicesCount += mesh->mFaces[i].mNumIndices;
    }
    meshObject.setIndicesCount(indicesCount);
    
    // Walk through each of the mesh's vertices
    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // We declare a placeholder std::vector since assimp uses its own std::vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // Texture Coordinates
        if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            
        meshObject.addVertex(vertex);
    }
    // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // Retrieve all indices of the face and store them in the indices std::vector
        for(GLuint j = 0; j < face.mNumIndices; j++)
            meshObject.addIndex(face.mIndices[j]);
    }
    // Process materials
    // assert(mesh->mMaterialIndex <= 1 && "The engine support just one material"); <= Assimp split the mesh into multiple meshes if it contains more then 1 material
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // Diffuse: texture_diffuseN
        // Specular: texture_specularN
        // Normal: texture_normalN
        
        uint texturesCount = material->GetTextureCount(aiTextureType_DIFFUSE) + material->GetTextureCount(aiTextureType_SPECULAR);
        //meshObject.setTexturesCount(texturesCount);
        m_texturesLoaded.reserve(texturesCount);
        
#ifdef MODEL_CONSOLE_TEX_INFO
        if (d_texCount > 0) d_texInfo += "\n";
        ++d_texCount;
#endif

        // 1. Diffuse maps
        loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse, meshObject);
        // 2. Specular maps
        loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular, meshObject);
    }
    
    //TODO
    meshObject.setMaterialShininess(16.f);
    
    // Return a mesh object created from the extracted mesh data
    meshObject.glue();
    return meshObject;
}

void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName, Mesh& mesh) {
    assert(mat->GetTextureCount(type) <= 1 && "The engine support just 1 diffuse and 1 one specular texture");
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        GLboolean skip = false;
        for(GLuint j = 0; j < m_texturesLoaded.size(); j++)
        {
            if(m_texturesLoaded[j].path == str)
            {
#ifdef MODEL_CONSOLE_TEX_INFO
                d_texInfo += "\t" + std::to_string(d_texCount) + ":" + std::to_string(typeName) + ":l \"" + m_texturesLoaded[j].path.C_Str() + "\"\n";
#endif
                mesh.addTexture(m_texturesLoaded[j].id, typeName);
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // If texture hasn't been loaded already, load it
#ifdef MODEL_CONSOLE_TEX_INFO
            d_texInfo += "\t" + std::to_string(d_texCount) + ":" + std::to_string(typeName) + ": \"" + (m_directory + "/" + str.C_Str()) + "\"\n";
#endif
            Texture texture;
            texture.id = Loader::loadTexture(m_directory + "/" + str.C_Str());
            texture.type = typeName;
            texture.path = str;
            mesh.addTexture(texture.id, typeName);
            m_texturesLoaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
}