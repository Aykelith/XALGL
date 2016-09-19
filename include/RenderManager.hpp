#ifndef RENDERMANAGER_HPP
#define RENDERMANAGER_HPP

#include <Global.hpp>

#include <unordered_map>
#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

#include <GL.hpp>
#include <Model.hpp>
#include <StaticShader.hpp>
#include <Camera.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>

#ifdef LIGHT_BOXES
    #include <DebugLight.hpp>
#endif

struct DrawableList {
    Model model;
    std::unique_ptr<StaticShader> shader;
    std::vector<glm::mat4> drawables;
    
    DrawableList() = default;
    
    // make it noncopyable
    DrawableList(DrawableList const&) = delete;
    DrawableList& operator=(DrawableList const&) = delete;

    // make it movable
    DrawableList(DrawableList&&) = default;
    DrawableList& operator=(DrawableList&&) = default;
};

class RenderManager {
public:
    RenderManager(const glm::mat4& projectionMatrix);
    
    void addDrawable(uint32_t id, uint32_t model);
    void addDrawable(uint32_t model, const glm::mat4& modelMatrix);
    
    void addModel(uint32_t id, const std::string& file);

    StaticShader* getShader(uint32_t id) { return m_drawableLists[id].shader.get(); }
    
    DirectionalLight* getDirectionalLight() { return &m_directionalLight; }
    PointLight*       addPointLight() {
        m_pointLights.emplace_back();
        return &m_pointLights.back();
    }
    
    void render(Camera &camera);
    
    //void initialize();
private:
    glm::mat4 m_projectionMatrix;
    
    std::unordered_map<uint32_t, DrawableList> m_drawableLists;
    
    DirectionalLight m_directionalLight;
    std::vector<PointLight> m_pointLights;
    // TODO
    // std::unordered_map<uint32_t, std::pair<std::unique_ptr<StaticShader>, uint32_t> > m_shaders;
    
#ifdef LIGHT_BOXES
    DebugLightShader d_lightShader;
    DebugLightModel d_lightModel;
#endif
    
};

#endif