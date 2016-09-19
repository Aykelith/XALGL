#include <RenderManager.hpp>

RenderManager::RenderManager(const glm::mat4& projectionMatrix)
    : m_projectionMatrix { projectionMatrix }
#ifdef LIGHT_BOXES
    , d_lightShader { projectionMatrix }
#endif
{ }

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
    m_drawableLists[id].shader = std::make_unique<StaticShader>(m_projectionMatrix);
}

void RenderManager::render(Camera &camera) {
    for (auto& list : m_drawableLists) {
        list.second.shader->start();
        
        list.second.shader->loadViewMatrix(camera);
        list.second.shader->loadViewPos(camera.getPosition());
        
        m_directionalLight.update(*list.second.shader);
        
        for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
            it->update(*list.second.shader);
        }
        
        assert(!checkErrors());
                
#ifdef EXPERIMENTAL_DRAW_SAME_MESH
        auto meshes = list.second.model.meshesCount();
        for (std::size_t i=0; i<meshes; ++i) {
            for (auto it = list.second.drawables.begin(); it != list.second.drawables.end(); ++it) {
                list.second.shader->loadModelMatrix(*it);
                list.second.model.drawE(*(list.second.shader), i);
            }
        }
#else
        for (auto it = list.second.drawables.begin(); it != list.second.drawables.end(); ++it) {
            list.second.shader->loadModelMatrix(*it);
            list.second.model.draw(*(list.second.shader));
        }
#endif
        
        list.second.shader->stop();
    }
    
#ifdef LIGHT_BOXES
    d_lightShader.start();
    d_lightShader.loadViewMatrix(camera);
    glBindVertexArray(d_lightModel.getVAO());
    for (auto it = m_pointLights.begin(); it != m_pointLights.end(); ++it) {
        glm::mat4 modelm;
        modelm = glm::translate(modelm, it->getPosition());
        d_lightShader.loadModelMatrix(modelm);
        d_lightShader.loadColor(it->getDiffuse());
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    d_lightShader.stop();
#endif
}
