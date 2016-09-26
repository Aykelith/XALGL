#ifndef SHADER_SETTINGS_HPP
#define SHADER_SETTINGS_HPP

namespace Shader {
    struct Settings {
        struct {
            // uint diffuseTextures = 0;
            
            struct {
                bool diffuseTextures = false;
                bool specularTextures = false;
                float constantShininess = -1.f; // -1 means is a uniform, not a constant
                
                void operator=(bool have) {
                    this->materials = 1;
                }
                
                explicit operator bool() const { return bool(materials); }
                
                int materials = 0;
            } material;
            struct {
                bool directionalLight = false;
                uint  pointLights = 0;
            } light;
        } fragment;
    };
}

#endif