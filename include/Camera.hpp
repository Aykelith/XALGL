#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Global.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/Window.hpp>

class Camera {
public:
    Camera() {

    }

    void update(/*sf::Window& window*/) {
        float d = 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            m_position += d * m_front;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_position += -d * m_front;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_position += glm::normalize(glm::cross(m_front, m_up)) * d;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_position += glm::normalize(glm::cross(m_front, m_up)) * -d;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto actualMousePosition = sf::Mouse::getPosition();

            if (m_firstTimePress) {
                m_oldMousePosition = actualMousePosition;
                m_firstTimePress = false;
            }

            GLfloat xoffset = actualMousePosition.x - m_oldMousePosition.x;
            GLfloat yoffset = m_oldMousePosition.y - actualMousePosition.y;
            //std::cout << xoffset << " " << yoffset << "\n";
            m_oldMousePosition = actualMousePosition;

            GLfloat sensitivity = 0.5;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            m_yaw   += xoffset;
            m_pitch += yoffset;

            if(m_pitch > 89.0f)
                m_pitch = 89.0f;
            if(m_pitch < -89.0f)
                m_pitch = -89.0f;

            glm::vec3 front;
            front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            front.y = sin(glm::radians(m_pitch));
            front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            m_front = glm::normalize(front);
        } else if (!m_firstTimePress) {
            m_firstTimePress = true;
        }
    }

    void setPosition(const glm::vec3& position) {
        m_position = position;
    }

    const glm::vec3& getPosition() const { return m_position; }

    glm::mat4 getViewMatrix() {
        // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
        // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
        return glm::lookAt(m_position,
          		           m_position + m_front,
          		           m_up
        );
    }

private:
    glm::vec3 m_position = glm::vec3(50.0f, 40.0f, 50.0f);
    glm::vec3 m_front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up       = glm::vec3(0.0f, 1.0f,  0.0f);

    sf::Vector2i m_oldMousePosition;
    bool m_firstTimePress = true;

    GLfloat m_yaw   = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
    GLfloat m_pitch =   0.0f;
};

#endif
