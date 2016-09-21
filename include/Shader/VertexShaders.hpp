#ifndef SHADER_VERTEXSHADERS_HPP
#define SHADER_VERTEXSHADERS_HPP

namespace Shader {
    static const char* vertex_defaultHeader = "\
#version 400 core\n\
\
layout (location = 0) in vec3 position;\
layout (location = 1) in vec3 normal;\
layout (location = 2) in vec2 texCoords;\
\
out vec2 TexCoords;\
\
uniform mat4 model;\
uniform mat4 view;\
uniform mat4 projection;\
";
    
    static const char* vertex_defaultMain = "\
gl_Position = projection * view * model * vec4(position, 1.f);\
TexCoords = texCoords;\
";
    
    static const char* vertex_lightHeader = "\
out vec3 FragPos;\
out vec3 Normal;\
";
    
    static const char* vertex_lightBody = "\
FragPos = vec3(model * vec4(position, 1.0f));\
Normal = normal;\
";
}

#endif