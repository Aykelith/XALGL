#include <Handler.hpp>

Handler::Handler()
    : m_world { m_window }
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    m_window.create(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
    m_window.setFramerateLimit(60);

    std::cout << m_window.getSettings().majorVersion << "." << m_window.getSettings().minorVersion << "\n";

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // wireframe

    glCullFace(GL_BACK);

    // For text
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_world.initialize();
}

int Handler::run() {
    m_clock.restart();

    bool running = true;
    while (running)
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                running = false;
            }

            m_world.events(event);
        }

        auto dt = m_clock.restart().asSeconds();
        m_world.update(dt);

        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_world.draw();

        // Swap buffers
        m_window.display();
    }

    //TODO: Clean

    m_window.close();

    return 0;
}
