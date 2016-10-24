#ifndef WORLD_HPP
#define WORLD_HPP

#include <Global.hpp>

#include <SFML/Window/Window.hpp>
#include <GL.hpp>

#include <Camera.hpp>
#include <RenderManager.hpp>
#include <Shader/ShaderGenerator.hpp>

#include <Debug.hpp>

class World {
public:
    World(sf::Window& window);

    void initialize();

    void events(const sf::Event& event);
    void update(float dt);
    void draw();

private:
    static constexpr float FOV = 45.f;
    static constexpr float NEAR_PLANE = 0.1f;
    static constexpr float FAR_PLANE = 1000.f;

private:
    sf::Window& m_window;
    glm::mat4 m_projectionMatrix;
    RenderManager m_renderManager;
    Camera m_camera;
};

#endif
