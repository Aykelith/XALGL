#include <GL.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

// #include <Loader.hpp>
// #include <RawModel.hpp>
#include <StaticShader.hpp>
// #include <TexturedModel.hpp>
// #include <ModelTexture.hpp>
// #include <Entity.hpp>
#include <Camera.hpp>
#include <RenderManager.hpp>

#include <Debug.hpp>

#include <random>

static const float FOV = 45.f;
static const float NEAR_PLANE = 0.1f;
static const float FAR_PLANE = 100.f;

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    sf::Window window(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);
    
    std::cout << window.getSettings().majorVersion << "." << window.getSettings().minorVersion << "\n";

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(FOV), (float)window.getSize().x/(float)window.getSize().y, NEAR_PLANE, FAR_PLANE);
    
    RenderManager renderManager(projectionMatrix);
    renderManager.addModel(0, "res/models/nanosuit/nanosuit.obj");
    
    auto dirLight = renderManager.getDirectionalLight();
    dirLight->setLightName("dirLight");
    dirLight->setAmbient({0.02f, 0.02f, 1.02f});
    dirLight->setDiffuse({1.f, 1.f, 1.f});
    dirLight->setSpecular({0.5f, 0.5f, 0.5f});
    dirLight->setDirection({-0.2f, -1.0f, -0.3f});
    
    auto pointLight1 = renderManager.addPointLight();
    pointLight1->setLightName("pointLights[0]");
    pointLight1->setAmbient({0.02f, 0.02f, 0.02f});
    pointLight1->setDiffuse({0.8f, 10.8f, 0.8f});
    pointLight1->setSpecular({1.0f, 1.0f, 1.0f});
    pointLight1->setConstant(4.f);
    pointLight1->setLinear(0.09f);
    pointLight1->setQuadratic(0.032f);
    pointLight1->setPosition({-4.0f,  2.0f, -12.0f});
    
    auto pointLight2 = renderManager.addPointLight();
    pointLight2->setLightName("pointLights[1]");
    pointLight2->setAmbient({0.02f, 0.02f, 0.02f});
    pointLight2->setDiffuse({0.8f, 10.8f, 0.8f});
    pointLight2->setSpecular({1.0f, 1.0f, 1.0f});
    pointLight2->setConstant(4.f);
    pointLight2->setLinear(0.09f);
    pointLight2->setQuadratic(0.032f);
    pointLight2->setPosition({0.7f,  0.2f,  2.0f});
    
    glm::mat4 modelm;
    modelm = glm::translate(modelm, glm::vec3(0.f, 0.f, -3.f));
    modelm = glm::scale(modelm, glm::vec3(0.5f, 0.5f, 0.5f));
    
    renderManager.addDrawable(0, modelm);
    
    // std::random_device rd;
    // std::mt19937 mt(rd());
    // std::uniform_real_distribution<float> distX(-10.f, 10.f);
    // std::uniform_real_distribution<float> distY(-10.f, 10.f);
    // std::uniform_real_distribution<float> distZ(-20.f, 0.f);
    // std::uniform_real_distribution<float> distS(0.1f, 0.5f);
    // 
    // for (int i=0; i<100; ++i) {
    //     glm::mat4 modelm;
    //     modelm = glm::translate(modelm, glm::vec3(distX(mt), distY(mt), distZ(mt))); // Translate it down a bit so it's at the center of the scene
    //     float s = distS(mt);
    //     modelm = glm::scale(modelm, glm::vec3(s, s, s));	// It's a bit too big for our scene, so scale it down
    //     renderManager.addDrawable(0, modelm);
    // }
    
    Camera camera;
    
    sf::Clock clock;
    
    glEnable(GL_DEPTH_TEST);
    
    bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                running = false;
            }
        }
        
        camera.update(/*window*/);
        
        // auto dt = clock.restart().asSeconds();
        
        // GLfloat radius = 10.0f;
        // GLfloat camX = sin(dt) * radius;
        // GLfloat camZ = cos(dt) * radius;
        // camera.setPosition({camX, 0.f, camZ});

        // Clear the screen to black
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderManager.render(camera);
        
        // Swap buffers
        window.display();
    }
    
    //loader.cleanUp();
    //shader->cleanUp();
    
    window.close();

    return 0;
}