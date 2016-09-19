#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <bitset>
#include <StaticShader.hpp>

template <std::size_t N = 3>
class Light {
public:
    Light() = default;
    
    void setLightName(const std::string& name) {
        m_lightName = name;
    }
    
    void setAmbient(const glm::vec3& ambient) {
        m_ambient = ambient;
        m_propertiesStatus[0] = 1;
    }
    
    void setDiffuse(const glm::vec3& diffuse) {
        m_diffuse = diffuse;
        m_propertiesStatus[1] = 1;
    }
    
    void setSpecular(const glm::vec3& specular) {
        m_specular = specular;
        m_propertiesStatus[2] = 1;
    }
    
    const glm::vec3& getDiffuse() const { return m_diffuse; }
    
    virtual void update(StaticShader& shader) {
        assert(!m_lightName.empty());
        if (m_propertiesStatus.any()) {
            implUpdate(shader);
            m_propertiesStatus.reset();
        } 
    }
protected:
    void implUpdate(StaticShader& shader) {
        if (m_propertiesStatus[0]) glUniform3f(shader.getUniformLocation((m_lightName + ".ambient").c_str()), m_ambient.x, m_ambient.y, m_ambient.z);
        if (m_propertiesStatus[1]) glUniform3f(shader.getUniformLocation((m_lightName + ".diffuse").c_str()), m_diffuse.x, m_diffuse.y, m_diffuse.z);
        if (m_propertiesStatus[2]) glUniform3f(shader.getUniformLocation((m_lightName + ".specular").c_str()), m_specular.x, m_specular.y, m_specular.z);
    }
    
protected:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    
    std::string m_lightName;
    
    std::bitset<N> m_propertiesStatus;
};

#endif