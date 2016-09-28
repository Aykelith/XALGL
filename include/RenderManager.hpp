#ifndef RENDERMANAGER_HPP
#define RENDERMANAGER_HPP

#include <Global.hpp>

#include <unordered_map>
#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

#include <GL.hpp>
#include <Model.hpp>
#include <Shader/ShaderGenerator.hpp>
#include <Camera.hpp>

#include <Light/DirectionalLight.hpp>
#include <Light/PointLight.hpp>

#include <Terrain/HeightmapTerrain.hpp>
#include <BlendMap.hpp>

#ifdef LIGHT_BOXES
    #include <Debug/Light.hpp>
#endif

struct DrawableList {
    Model model;
    Shader::ShaderProgram shader;
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
    RenderManager() = default;
    
    void initialize();
    
    void setProjectionMatrix(const glm::mat4& projectionMatrix) { 
        m_projectionMatrix = projectionMatrix; 
        m_terrain.setProjectionMatrix(projectionMatrix);
        
#ifdef LIGHT_BOXES
        d_lightShader.setProjectionMatrix(projectionMatrix);
#endif
    }
    
    void addDrawable(uint32_t id, uint32_t model);
    void addDrawable(uint32_t model, const glm::mat4& modelMatrix);
    
    void addModel(uint32_t id, const std::string& file);

    void addShader(uint32_t id, const Shader::Settings& settings);
    void addShader(uint32_t id, const Shader::Settings& settings, const std::string& vertexFile, const std::string& fragmentFile);
    
    Light::DirectionalLight* getDirectionalLight() { return &m_directionalLight; }
    Light::PointLight*       addPointLight() {
        m_pointLights.emplace_back();
        return &m_pointLights.back();
    }
    
    void draw(Camera &camera);
    
    //void initialize();
private:
    glm::mat4 m_projectionMatrix;
    
    std::unordered_map<uint32_t, DrawableList> m_drawableLists;
    
    Light::DirectionalLight m_directionalLight;
    std::vector<Light::PointLight> m_pointLights;
    
    Terrains::HeightmapTerrain m_terrain;
    BlendMap m_terrainBlendmap;
    
    // DEBUG
#ifdef LIGHT_BOXES
    Debug::LightShader d_lightShader;
    Debug::LightModel d_lightModel;
#endif
    
};

#endif