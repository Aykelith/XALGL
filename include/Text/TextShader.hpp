#ifndef TEXT_TEXTSHADER_HPP
#define TEXT_TEXTSHADER_HPP

#include <Shader/ShaderProgram.hpp>
#include <Text/Font.hpp>

#include <Shader/Uniforms.hpp>

class TextShader: public Shader::ShaderProgram {
public:
    enum class Uniform : int {
        TEXTURE = Shader::Impl::TEXT_START,
        TEXT_COLOR,
        _COUNT
    };
    static_assert(static_cast<int>(Uniform::_COUNT) == Shader::Impl::TEXT_START + Shader::Impl::TEXT_COUNT);
    
public:
    TextShader() = default;
    
    void initialize() {
        // TODO: check if the shaders were loaded, maybe use bitset
        // TODO: offer more flexibility for inheritence
        createProgram();
        bindAttribute(0, "position");
        link();
        storeUniformLocation(static_cast<int>(Shader::Uniform::PROJECTION_MATRIX), "projection");
        storeUniformLocation(static_cast<int>(Uniform::TEXTURE), "text");
        storeUniformLocation(static_cast<int>(Uniform::TEXT_COLOR), "textColor");
        assert(!checkErrors());
    }
    
    void setProjection(const glm::mat4& projection) {
        start();
        loadMatrix(static_cast<int>(Shader::Uniform::PROJECTION_MATRIX), projection);
        stop();
    }
};

#endif