#ifndef TERRAINS_BLENDMAP_HPP
#define TERRAINS_BLENDMAP_HPP

#include <Shader/ShaderGenerator.hpp>
#include <Loader.hpp>

#include <Shader/Uniforms.hpp>

class BlendMap {
public:
    enum class Uniform : int {
        BLENDMAP_TEXTURE = Shader::Impl::BLENDMAP_START,
        BLACK_TEXTURE,
        RED_TEXTURE,
        GREEN_TEXTURE,
        BLUE_TEXTURE,
        _COUNT
    };
    static_assert(static_cast<int>(Uniform::_COUNT) == Shader::Impl::BLENDMAP_START + Shader::Impl::BLENDMAP_COUNT);
    
    enum class ColorChannel : int {
        BLACK = 0,
        RED,
        GREEN,
        BLUE,
        _COUNT
    };
    
public:
    BlendMap() = default;
    
    void setTexture(ColorChannel channel, const std::string& file) {
        m_settedTextures[static_cast<int>(channel)] = 1;
        m_textures[static_cast<int>(channel)] = Loader::loadTexture(file, GL_REPEAT, GL_REPEAT);
    }
    
    void setBlendMap(const std::string& file) {
        m_blendmap = Loader::loadTexture(file);
    }
    
    void initialize(Shader::ShaderProgram& shader) {
        shader.storeUniformLocation(static_cast<int>(Uniform::BLENDMAP_TEXTURE), "blendmapTexture");
        for (int i=0; i<static_cast<int>(ColorChannel::_COUNT); ++i) {
            if (m_settedTextures[i]) { 
                int uniformValue = getUniformValue(i);
                shader.storeUniformLocation(uniformValue, getUniformName(uniformValue));
                std::cout << uniformValue << " " << getUniformName(uniformValue) << "\n";
            }
        }
    }
    
    void start(Shader::ShaderProgram& shader) {
        glActiveTexture(GL_TEXTURE0);
        shader.loadInt(static_cast<int>(Uniform::BLENDMAP_TEXTURE), 0);
        glBindTexture(GL_TEXTURE_2D, m_blendmap);
        
        for (int i=0, tIndex = 0; i<static_cast<int>(ColorChannel::_COUNT); ++i) {
            // Starting for the GL_TEXTURE1 because GL_TEXTURE0 is the blendmap
            if (m_settedTextures[i]) {
                glActiveTexture(GL_TEXTURE1 + tIndex);
                shader.loadInt(getUniformValue(i), 1 + tIndex);
                glBindTexture(GL_TEXTURE_2D, m_textures[i]);
                ++tIndex;
            }
        }
    }
    
    void stop() {
        // First texture spot is the blendmap
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        for (int i=0, tIndex = 0; i<static_cast<int>(ColorChannel::_COUNT); ++i) {
            // Starting for the GL_TEXTURE1 because GL_TEXTURE0 is the blendmap
            if (m_settedTextures[i]) {
                glActiveTexture(GL_TEXTURE1 + tIndex);
                glBindTexture(GL_TEXTURE_2D, 0);
                ++tIndex;
            }
        }
    }
    
private:
    int getUniformValue(int /*ColorChannel*/ channel) {
        return static_cast<int>(Uniform::BLENDMAP_TEXTURE) + 1 + channel;
    }
    
    const char* getUniformName(int /*Uniform*/ uniform) {
        switch (uniform) {
            case static_cast<int>(Uniform::BLACK_TEXTURE): return "blackTexture";
            case static_cast<int>(Uniform::RED_TEXTURE): return "redTexture";
            case static_cast<int>(Uniform::GREEN_TEXTURE): return "greenTexture";
            case static_cast<int>(Uniform::BLUE_TEXTURE): return "blueTexture";
            default: {
                assert(false && "Uniform value is invalid");
                return "";
            }
        }
    }
    
private:
    std::array<GLuint, static_cast<int>(ColorChannel::_COUNT)> m_textures;
    std::bitset<static_cast<int>(ColorChannel::_COUNT)> m_settedTextures;
    GLuint m_blendmap;
};

#endif