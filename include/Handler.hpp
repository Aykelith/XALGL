#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <Global.hpp>

#include <SFML/Window/Window.hpp>
#include <SFML/System/Clock.hpp>
#include <GL.hpp>

#include <World.hpp>

class Handler {
public:
    Handler();
    
    int run();
    
private:
    sf::Window m_window;
    sf::Clock m_clock;
    
    World m_world;
};

#endif