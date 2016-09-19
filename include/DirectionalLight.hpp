#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include <Light.hpp>

class DirectionalLight: public Light<4> {
public:
    DirectionalLight() = default;
    
    void setDirection(const glm::vec3& direction) {
        m_direction = direction;
        m_propertiesStatus[3] = 1;
    }
    
    void update(StaticShader& shader) { 
        assert(!m_lightName.empty());
        if (m_propertiesStatus.any()) {
            Light::implUpdate(shader);
            implUpdate(shader);
            m_propertiesStatus.reset();
        } 
    }
private:
    void implUpdate(StaticShader& shader) {
        if (m_propertiesStatus[3]) glUniform3f(shader.getUniformLocation((m_lightName + ".direction").c_str()), m_direction.x, m_direction.y, m_direction.z);
    }
    
private:
    glm::vec3 m_direction;
};

#endif