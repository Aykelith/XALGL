#ifndef TEXT_FONT_HPP
#define TEXT_FONT_HPP

#include <Global.hpp>

#include <GL.hpp>
#include <Rect.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

class Font {
public:
    struct Glyph {
        Glyph(): advance { 0 } { }
        
        float     advance;
        FloatRect bounds;
        IntRect   textureRect;
    };
    
    
    
public:
    Font() = default;
    
    void loadFromFile(const std::string& file) {
        // FreeType
        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        // Load font as face
        FT_Face face;
        if (FT_New_Face(ft, file.c_str(), 0, &face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

        // Load first 128 characters of ASCII set
        for (GLubyte c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            
            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            m_characters.emplace(c, character);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        // Destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
    
    Character& getCharacter(GLchar index) { 
        assert(m_characters.find(index) != m_characters.end());
        return m_characters[index];
    }
    
private:
    struct Row
    {
        Row(unsigned int rowTop, unsigned int rowHeight) : width(0), top(rowTop), height(rowHeight) {}

        unsigned int width;  ///< Current width of the row
        unsigned int top;    ///< Y position of the row into the texture
        unsigned int height; ///< Height of the row
    };
    
    std::unordered_map<GLchar, Character> m_characters;
};

#endif