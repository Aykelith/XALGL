#include <RenderManager.hpp>

RenderManager::RenderManager(const glm::mat4& projectionMatrix)
    : m_projectionMatrix { projectionMatrix }
    , m_terrain { 0, -1, projectionMatrix }
#ifdef LIGHT_BOXES
    , d_lightShader { projectionMatrix }
#endif
{ 
    m_terrainBlendmap.setTexture(ColorChannel::BLACK, "res/grassy2.png");
    m_terrainBlendmap.setTexture(ColorChannel::RED, "res/mud.png");
    m_terrainBlendmap.setTexture(ColorChannel::GREEN, "res/grassFlowers.png");
    m_terrainBlendmap.setTexture(ColorChannel::BLUE, "res/path.png");
    m_terrainBlendmap.setBlendMap("res/blendMap.png");
    m_terrainBlendmap.initialize(m_terrain.getShader());
    std::cout << "Out\n";
}

void RenderManager::addDrawable(uint32_t id, uint32_t model) {
    
}

void RenderManager::addDrawable(uint32_t model, const glm::mat4& modelMatrix) {
    assert(m_drawableLists.find(model) != m_drawableLists.end() && "Modelul nu a fost gasit");
    
    m_drawableLists[model].drawables.emplace_back(modelMatrix);
}

void RenderManager::addModel(uint32_t id, const std::string& file) {
    assert(m_drawableLists.find(id) == m_drawableLists.end() && "Modelul a fost deja introdus");
    
    m_drawableLists.emplace(std::make_pair(
        id, 
        DrawableList()
    ));
    
    m_drawableLists[id].model = Model(file);
}

void RenderManager::addShader(uint32_t id, const Shader::Settings& settings) {
    auto& shader = m_drawableLists[id].shader;
    Shader::generateShaderFiles(shader, settings);
    Shader::initializeShader(shader, settings);
    shader.start();
    shader.loadMatrix(Shader::Uniform::PROJECTION_MATRIX, m_projectionMatrix);
    shader.stop();
}

void RenderManager::addShader(uint32_t id, const Shader::Settings& settings, const std::string& vertexFile, const std::string& fragmentFile) {
    assert(!checkErrors(__LINE__, __FILE__));
    auto& shader = m_drawableLists[id].shader;
    shader.loadShader(vertexFile, GL_VERTEX_SHADER);
    shader.loadShader(fragmentFile, GL_FRAGMENT_SHADER);
    Shader::initializeShader(shader, settings);
    shader.start();
    shader.loadMatrix(Shader::Uniform::PROJECTION_MATRIX, m_projectionMatrix);
    shader.stop();
}

void RenderManager::render(Camera &camera) {
    auto& terrainShader = m_terrain.getShader();
    terrainShader.start();
    m_terrainBlendmap.start(terrainShader);
    terrainShader.loadMatrix(Shader::Uniform::VIEW_MATRIX, camera.getViewMatrix());
    
    // terrainShader.loadVector3(Shader::Uniform::VIEW_POS, camera.getPosition());
    
    // m_directionalLight.update(terrainShader);
    // 
    // for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
    //     it->update(terrainShader);
    // }
    
    m_terrain.draw();
    m_terrainBlendmap.stop();
    terrainShader.stop();
    assert(!checkErrors());
    
    for (auto& list : m_drawableLists) {
        list.second.shader.start();
        
        list.second.shader.loadMatrix(Shader::Uniform::VIEW_MATRIX, camera.getViewMatrix());
        list.second.shader.loadVector3(Shader::Uniform::VIEW_POS, camera.getPosition());
        
        m_directionalLight.update(list.second.shader);

        for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
            it->update(list.second.shader);
        }
        
        assert(!checkErrors());
                
#ifdef EXPERIMENTAL_DRAW_SAME_MESH
        auto meshes = list.second.model.meshesCount();
        for (std::size_t i=0; i<meshes; ++i) {
            for (auto it = list.second.drawables.begin(); it != list.second.drawables.end(); ++it) {
                list.second.shader.loadMatrix(Shader::Uniform::MODEL_MATRIX, *it);
                list.second.model.drawE(list.second.shader, i);
            }
        }
#else
        for (auto it = list.second.drawables.begin(); it != list.second.drawables.end(); ++it) {
            list.second.shader.loadMatrix(Shader::Uniform::MODEL_MATRIX, *it);
            list.second.model.draw(list.second.shader);
        }
        
        assert(!checkErrors());
#endif
        
        list.second.shader.stop();
    }
    
    m_directionalLight.changesSaved();
    
    for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
        it->changesSaved();
    }
    
#ifdef LIGHT_BOXES
    d_lightShader.start();
    d_lightShader.loadMatrix(Shader::Uniform::VIEW_MATRIX, camera.getViewMatrix());
    glBindVertexArray(d_lightModel.getVAO());
    for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
        glm::mat4 modelm;
        modelm = glm::translate(modelm, it->getPosition());
        d_lightShader.loadMatrix(Shader::Uniform::MODEL_MATRIX, modelm);
        d_lightShader.loadVector3(Debug::LightShader::UNIFORM_COLOR, it->getDiffuse());
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    d_lightShader.stop();
#endif
}
