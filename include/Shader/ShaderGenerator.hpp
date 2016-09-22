#ifndef SHADER_SHADERGENERATOR_HPP
#define SHADER_SHADERGENERATOR_HPP

#include <Global.hpp>

#include <Shader/VertexShaders.hpp>
#include <Shader/FragmentShaders.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Shader/Settings.hpp>

namespace Shader {
    static const char* shader_mainStart = "void main(){";
    static const char* shader_mainEnd   = "}";
    static const char* shader_normalEndKey = ";";
    
    inline void generateShaderFiles(ShaderProgram& shader, const Settings& settings) {
        bool haveLights = settings.fragment.light.directionalLight || settings.fragment.light.pointLights;
        
        // HEADER
        //======================================================================
        std::string vertexData = vertex_defaultHeader;
        std::string fragmentData = fragment_defaultHeader;
        
        if (settings.fragment.diffuseTextures) {
            for (uint i=1; i<=settings.fragment.diffuseTextures; ++i) {
                fragmentData += fragment_diffuseTexture + std::to_string(i) + shader_normalEndKey;
            }
        }
        
        if (settings.fragment.material) {
            fragmentData += fragment_material;
        }
        
        if (haveLights) {
            vertexData += vertex_lightHeader;
            fragmentData += fragment_lightDefaultHeader;
            
            if (settings.fragment.light.directionalLight) {
                fragmentData += fragment_directionalLight;
            }
            
            if (settings.fragment.light.pointLights) {
                fragmentData += fragment_pointLightNumber + std::to_string(settings.fragment.light.pointLights) + fragment_pointLightNumberEndKey;
                fragmentData += fragment_pointLight;
            }
        }
        //======================================================================
        
        // BODY
        //======================================================================
        vertexData += std::string() + shader_mainStart + vertex_defaultMain;
        fragmentData += shader_mainStart;
        
        if (haveLights) {
            vertexData += vertex_lightBody;
            fragmentData += fragment_lightDefaultMainStart;
            
            if (settings.fragment.light.directionalLight) {
                fragmentData += fragment_directionalLightMain;
            }
            
            if (settings.fragment.light.pointLights) {
                fragmentData += fragment_pointLightMain;
            }
        }
        
        // Body ending part
        if (haveLights) {
            fragmentData += fragment_lightDefaultMainEnd;
        } else {
            
        }
        
        vertexData += shader_mainEnd;
        fragmentData += shader_mainEnd;
        //======================================================================
        
#ifdef SHADER_OUTPUT
    #include <functional>
    #include <iostream>
    
        auto d_VertexData = vertexData;
        auto d_FragmentData = fragmentData;
        
        auto d_ = [](std::string& data, char key){
            uint index = data.find(key); 
            while (index != std::string::npos) {
                data.insert(index+1, 1, '\n');
                index = data.find(key, index+1);
            }
        };
        
        d_(d_VertexData, ';');
        d_(d_FragmentData, ';');
        d_(d_VertexData, '{');
        d_(d_FragmentData, '{');
        std::cout << "= VERTEX ==========================\n"
                  << d_VertexData 
                  << "\n= FRAGMENT ========================\n" 
                  << d_FragmentData 
                  << "\n===================================\n";
#endif

        shader.loadShader(vertexData.c_str(), GL_VERTEX_SHADER);
        shader.loadShader(fragmentData.c_str(), GL_FRAGMENT_SHADER);
    }
    
    inline void initializeShader(ShaderProgram& shader, const Settings& settings) {
        shader.createProgram();
        shader.bindAttribute(0, "position");
        shader.bindAttribute(1, "normal");
        shader.bindAttribute(2, "texCoords");
        shader.link();
        shader.storeUniformLocation(Uniform::MODEL_MATRIX, "model");
        shader.storeUniformLocation(Uniform::VIEW_MATRIX, "view");
        shader.storeUniformLocation(Uniform::PROJECTION_MATRIX, "projection");
        
        if (settings.fragment.light.directionalLight || settings.fragment.light.pointLights) {
            shader.storeUniformLocation(Uniform::VIEW_POS, "viewPos");
        }
        
        if (settings.fragment.material) {
            shader.storeUniformLocation(Uniform::MATERIAL_SHININESS, "material.shininess");
            shader.storeUniformLocation(Uniform::MATERIAL_DIFFUSE_TEXTURE, "material.diffuse");
            shader.storeUniformLocation(Uniform::MATERIAL_SPECULAR_TEXTURE, "material.specular");
        }
        
        assert(!checkErrors());
    }
}

#endif