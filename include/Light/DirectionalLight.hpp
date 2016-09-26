#ifndef LIGHT_DIRECTIONALLIGHT_HPP
#define LIGHT_DIRECTIONALLIGHT_HPP

#include <Global.hpp>

#include <Light/Light.hpp>

namespace Light {    
    class DirectionalLight: public Light<4> {
    public:
        DirectionalLight() = default;
        
        void setDirection(const glm::vec3& direction) {
            m_direction = direction;
            m_propertiesStatus[3] = 1;
        }
        
        void update(Shader::ShaderProgram& shader) { 
            assert(!m_lightName.empty());
            if (m_propertiesStatus.any()) {
                Light::implUpdate(shader);
                implUpdate(shader);
                m_changes = true;
            } 
        }
    private:
        void implUpdate(Shader::ShaderProgram& shader) {
            if (m_propertiesStatus[3]) glUniform3f(shader.getUniformLocation((m_lightName + ".direction").c_str()), m_direction.x, m_direction.y, m_direction.z);
        }
        
    private:
        glm::vec3 m_direction;
    };
}

#endif