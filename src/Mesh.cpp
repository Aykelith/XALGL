#include <Mesh.hpp>

#ifdef MODEL_CONSOLE_INFO
    #include <iostream>
    
    #ifdef MODEL_CONSOLE_MESH_INFO
        #include <sstream>
    #endif
#endif

void Mesh::draw(Shader::ShaderProgram& shader) 
{
    // Bind appropriate textures
    // Tip for multi texture: glActiveTexture(GL_TEXTURE0 + i);
    if (m_material.haveDiffuseTexture) {
        glActiveTexture(GL_TEXTURE0);
        shader.loadInt(static_cast<int>(Shader::Uniform::MATERIAL_DIFFUSE_TEXTURE), 0);
        glBindTexture(GL_TEXTURE_2D, m_material.diffuseTexture);
    }
    
    if (m_material.haveSpecularTexture) {
        glActiveTexture(GL_TEXTURE1);
        shader.loadInt(static_cast<int>(Shader::Uniform::MATERIAL_SPECULAR_TEXTURE), 1);
        glBindTexture(GL_TEXTURE_2D, m_material.specularTexture);
    }
    
    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
    if (m_material.haveShininess) shader.loadFloat(static_cast<int>(Shader::Uniform::MATERIAL_SHININESS), m_material.shininess);

    // Draw mesh
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Always good practice to set everything back to defaults once configured.
    if (m_material.haveDiffuseTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    if (m_material.haveSpecularTexture) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

#ifdef EXPERIMENTAL_DRAW_SAME_MESH
void Mesh::bindTextures(StaticShader& shader) {
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        std::string number;
        TextureType name = m_textures[i].type;
        if(name == TextureType::Diffuse) {
            number = std::to_string(diffuseNr); // Transfer GLuint to stream
            ++diffuseNr;
        }
        else if(name == TextureType::Specular) {
            number = std::to_string(specularNr); // Transfer GLuint to stream
            ++specularNr;
        }
        // Now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.Program, (getTextureTypeName(name) + number).c_str()), i);
        // And finally bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
}

void Mesh::unbindTextures() {
    for (GLuint i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Mesh::drawE(StaticShader& shader) {
    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
    shader.loadFloat(static_cast<int>(Shader::Uniform::MATERIAL_SHININESS), 16.0f);

    // Draw mesh
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
#endif

void Mesh::glue()
{
#ifdef MODEL_CONSOLE_MESH_INFO
    std::stringstream ss;
    ss << "\t Mesh: V:" << m_vertices.size() << " I:" << m_indices.size() << " T_D:" << m_material.haveDiffuseTexture << " T_S:" << m_material.haveSpecularTexture << "\n";
    
    // if (!m_textures.empty()) {
    //     std::size_t diffuseCount = 0, specularCount = 0;
    //     for(GLuint i = 0; i < m_textures.size(); i++) {
    //         if (m_textures[i].type == TextureType::Diffuse) ++diffuseCount;
    //         else if (m_textures[i].type == TextureType::Specular) ++specularCount;
    //     }
    //     ss << "\t\tdiffuse: " << diffuseCount << " specular:" << specularCount << "\n";
    // }
    
    d_info = ss.str();
#endif
    
    // Create buffers/arrays
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}