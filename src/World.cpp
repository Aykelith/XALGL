#include <World.hpp>

World::World(sf::Window& window)
    : m_window { window }
{

}

void World::initialize() {
    m_projectionMatrix = glm::perspective(glm::radians(FOV), (float)m_window.getSize().x/(float)m_window.getSize().y, NEAR_PLANE, FAR_PLANE);

    m_renderManager.initialize();
    m_renderManager.setProjectionMatrix(m_projectionMatrix);

    Shader::Settings shaderSettings;
    shaderSettings.fragment.material = true;
    shaderSettings.fragment.material.diffuseTextures = true;
    shaderSettings.fragment.material.specularTextures = true;
    shaderSettings.fragment.light.directionalLight = true;
    shaderSettings.fragment.light.pointLights = 2;
    // shaderSettings.fragment.diffuseTextures = 1;

    m_renderManager.addModel(0, "res/models/nanosuit/nanosuit.obj");
    m_renderManager.addShader(0, shaderSettings, "res/default.vert", "res/default.frag");

    auto dirLight = m_renderManager.getDirectionalLight();
    dirLight->setLightName("dirLight");
    dirLight->setAmbient({0.02f, 0.02f, 1.02f});
    dirLight->setDiffuse({1.f, 1.f, 1.f});
    dirLight->setSpecular({0.5f, 0.5f, 0.5f});
    dirLight->setDirection({-0.2f, -1.0f, -0.3f});

    auto pointLight1 = m_renderManager.addPointLight();
    pointLight1->setLightName("pointLights[0]");
    pointLight1->setAmbient({0.02f, 0.02f, 0.02f});
    pointLight1->setDiffuse({0.8f, 10.8f, 0.8f});
    pointLight1->setSpecular({1.0f, 1.0f, 1.0f});
    pointLight1->setConstant(4.f);
    pointLight1->setLinear(0.09f);
    pointLight1->setQuadratic(0.032f);
    pointLight1->setPosition({-4.0f,  20.0f, -12.0f});

    auto pointLight2 = m_renderManager.addPointLight();
    pointLight2->setLightName("pointLights[1]");
    pointLight2->setAmbient({0.02f, 0.02f, 0.02f});
    pointLight2->setDiffuse({10.f, 0.f, 0.8f});
    pointLight2->setSpecular({1.0f, 1.0f, 1.0f});
    pointLight2->setConstant(4.f);
    pointLight2->setLinear(0.09f);
    pointLight2->setQuadratic(0.032f);
    pointLight2->setPosition({10.f,  30.f,  10.0f});

    glm::mat4 modelm;
    modelm = glm::translate(modelm, glm::vec3(0.f, 30.f, -3.f));
    modelm = glm::scale(modelm, glm::vec3(0.5f, 0.5f, 0.5f));

    m_renderManager.addDrawable(0, modelm);
}

void World::events(const sf::Event& event) {

}

void World::update(float dt) {
    m_camera.update(/*window*/);
}

void World::draw() {
    m_renderManager.draw(m_camera);
    assert(!checkErrors());
}
