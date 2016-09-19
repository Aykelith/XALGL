#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include <GL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformable {
public:
    Transformable() = default;
    
    virtual prepare() {
        
    };
    
protected:
    glm::mat4 m_model;    
};

#endif