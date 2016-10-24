#ifndef WORLD_HPP
#define WORLD_HPP

#include <Global.hpp>

#include <SFML/Window/Window.hpp>
#include <GL.hpp>

#include <Camera.hpp>
#include <RenderManager.hpp>
#include <Shader/ShaderGenerator.hpp>

#include <Debug.hpp>

// 3d
#include <cmath>
#define degreesToRadians(x) x*(3.141592f/180.0f)

// #include <Text/Font.hpp>
// #include <Text/TextShader.hpp>
// #include <Text/Text.hpp>

class World {
public:
    World(sf::Window& window);

    void initialize();

    void events(const sf::Event& event);
    void update(float dt);
    void draw();

    // 3D
    glm::mat4 leftMat;
    glm::mat4 rightMat;

    Camera leftCamera;
    void stereoscopicEffect() {
        int eyeSeparation = 3;
        int focalLength = 15;
        float _fov = 2.f * atan( tan(degreesToRadians(FOV)) * 0.5f);
        float aspect = (float)m_window.getSize().x/(float)m_window.getSize().y;

        float ndfl = NEAR_PLANE / focalLength;
        float halfFocalHeight = tan(_fov * 0.5f) * focalLength;
        float halfFocalWidth = halfFocalHeight * 0.5f * aspect;

        float top = halfFocalHeight * ndfl;
        float bottom = -top;
        float innerFactor = (halfFocalWidth + eyeSeparation / 2.f) / (halfFocalWidth * 2.f);
        float outerFactor = 1.f - innerFactor;

        float outer = halfFocalWidth * 2.f * ndfl * outerFactor;
        float inner = halfFocalWidth * 2.f * ndfl * innerFactor;

        leftMat = glm::frustum(-outer, inner, bottom, top, NEAR_PLANE, FAR_PLANE);
        glm::translate(leftMat, {-eyeSeparation/2.f, 0.f, 0.f});
    }

private:
    static constexpr float FOV = 45.f;
    static constexpr float NEAR_PLANE = 0.1f;
    static constexpr float FAR_PLANE = 1000.f;

private:
    sf::Window& m_window;
    glm::mat4 m_projectionMatrix;
    RenderManager m_renderManager;
    Camera m_camera;

    // Font m_font;
    // TextShader m_textShader;
    // Text m_text;
};

#endif
