#ifndef LIGHT_POINTLIGHT_HPP
#define LIGHT_POINTLIGHT_HPP

#include <Global.hpp>

#include <Light/Light.hpp>

namespace Light {    
    class PointLight: public Light<7> {
    public:
        PointLight() = default;
        
        void setPosition(const glm::vec3& position) {
            m_position = position;
            m_propertiesStatus[3] = 1;
        }
        
        void setConstant(float constant) {
            m_constant = constant;
            m_propertiesStatus[4] = 1;
        }
        
        void setLinear(float linear) {
            m_linear = linear;
            m_propertiesStatus[5] = 1;
        }
        
        void setQuadratic(float quadratic) {
            m_quadratic = quadratic;
            m_propertiesStatus[6] = 1;
        }
        
        const glm::vec3& getPosition() const { return m_position; }
        
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
            if (m_propertiesStatus[3]) glUniform3f(shader.getUniformLocation((m_lightName + ".position").c_str()), m_position.x, m_position.y, m_position.z);
            if (m_propertiesStatus[4]) glUniform1f(shader.getUniformLocation((m_lightName + ".constant").c_str()), m_constant);
            if (m_propertiesStatus[5]) glUniform1f(shader.getUniformLocation((m_lightName + ".linear").c_str()), m_linear);
            if (m_propertiesStatus[6]) glUniform1f(shader.getUniformLocation((m_lightName + ".quadratic").c_str()), m_quadratic);
        }
        
    private:
        glm::vec3 m_position;
        float     m_constant;
        float     m_linear;
        float     m_quadratic;
    };
}

#endif