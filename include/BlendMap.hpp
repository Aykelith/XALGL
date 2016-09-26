#ifndef TERRAINS_BLENDMAP_HPP
#define TERRAINS_BLENDMAP_HPP

#include <Shader/ShaderGenerator.hpp>
#include <Loader.hpp>

enum Uniform {
    BLENDMAP_TEXTURE = Shader::Uniform::_COUNT,
    BLACK_TEXTURE,
    RED_TEXTURE,
    GREEN_TEXTURE,
    BLUE_TEXTURE
};

enum ColorChannel {
    BLACK = 0,
    RED,
    GREEN,
    BLUE,
    _COUNT
};

class BlendMap {
public:
    BlendMap() = default;
    
    void setTexture(ColorChannel channel, const std::string& file) {
        m_settedTextures[channel] = 1;
        m_textures[channel] = Loader::loadTexture(file, GL_REPEAT, GL_REPEAT);
    }
    
    void setBlendMap(const std::string& file) {
        m_blendmap = Loader::loadTexture(file);
    }
    
    void initialize(Shader::ShaderProgram& shader) {
        shader.storeUniformLocation(Uniform::BLENDMAP_TEXTURE, "blendmapTexture");
        for (int i=0; i<ColorChannel::_COUNT; ++i) {
            if (m_settedTextures[i]) { 
                int uniformValue = getUniformValue(i);
                shader.storeUniformLocation(uniformValue, getUniformName(uniformValue));
                std::cout << uniformValue << " " << getUniformName(uniformValue) << "\n";
            }
        }
    }
    
    void start(Shader::ShaderProgram& shader) {
        glActiveTexture(GL_TEXTURE0);
        shader.loadInt(Uniform::BLENDMAP_TEXTURE, 0);
        glBindTexture(GL_TEXTURE_2D, m_blendmap);
        
        for (int i=0, tIndex = 0; i<ColorChannel::_COUNT; ++i) {
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
        
        for (int i=0, tIndex = 0; i<ColorChannel::_COUNT; ++i) {
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
        return Uniform::BLENDMAP_TEXTURE + 1 + channel;
    }
    
    const char* getUniformName(int /*Uniform*/ uniform) {
        switch (uniform) {
            case Uniform::BLACK_TEXTURE: return "blackTexture";
            case Uniform::RED_TEXTURE: return "redTexture";
            case Uniform::GREEN_TEXTURE: return "greenTexture";
            case Uniform::BLUE_TEXTURE: return "blueTexture";
            default: {
                assert(false && "Uniform value is invalid");
                return "";
            }
        }
    }
    
private:
    std::array<GLuint, ColorChannel::_COUNT> m_textures;
    std::bitset<ColorChannel::_COUNT> m_settedTextures;
    GLuint m_blendmap;
};

#endif