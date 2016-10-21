#ifndef TEXT_TEXT_HPP
#define TEXT_TEXT_HPP

#include <Text/TextShader.hpp>
#include <Text/Font.hpp>

class Text {
public:
    Text() = default;
    
    void initialize() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    void setFont(Font& font) {
        if (m_font != &font) {
            m_font = &font;
        }
    }
    
    void setPosition(const glm::vec2 position) {
        m_position = position;
        m_updateText = true;
    }
    
    void setString(const std::string& text) {
        m_text = text;
        m_updateText = true;
    }
    
    void setColor(const glm::vec3& color) {
        m_color = glm::normalize(color);
        m_updateColor = true;
    }
    
    void draw(TextShader& shader) {
        assert(m_font != nullptr);
        
        if (m_updateColor) {
            shader.loadVector3(static_cast<int>(TextShader::Uniform::TEXT_COLOR), m_color);
            m_updateColor = false;
        }
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(m_VAO);

        float x = m_position[0];

        // Iterate through all characters
        std::string::const_iterator c;
        for (c = m_text.begin(); c != m_text.end(); c++) 
        {
            auto& ch = m_font->getCharacter(*c);

            GLfloat xpos = x + ch.Bearing.x * 1 /* scale */;
            GLfloat ypos = m_position[1] - (ch.Size.y - ch.Bearing.y) * 1 /* scale */;

            GLfloat w = ch.Size.x * 1 /* scale */;
            GLfloat h = ch.Size.y * 1 /* scale */;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },            
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }           
            };
            
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            assert(!checkErrors());
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
            assert(!checkErrors());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * 1 /* scale */; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        if (m_updateText) m_updateText = false;
    }
    
private:
    GLuint m_VAO;
    GLuint m_VBO;
    
    std::string m_text;
    glm::vec3 m_color = {1.f, 1.f, 1.f};
    glm::vec2 m_position = {0.f, 0.f};
    Font* m_font = nullptr;
    
    bool m_updateColor = false;
    bool m_updateText = false;
};

#endif