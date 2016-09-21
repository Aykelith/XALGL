#ifndef SHADER_SETTINGS_HPP
#define SHADER_SETTINGS_HPP

namespace Shader {
    struct Settings {
        struct {
            uint diffuseTextures = 0;
            
            bool material = false;
            struct {
                bool directionalLight = false;
                uint  pointLights = 0;
            } light;
        } fragment;
    };
}

#endif