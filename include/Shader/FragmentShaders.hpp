#ifndef SHADER_FRAGMENTSHADERS_HPP
#define SHADER_FRAGMENTSHADERS_HPP

namespace Shader {
    static const char* fragment_defaultHeader = "\
#version 400 core\n\
\
in vec2 TexCoords;\
\
out vec4 outColor;\
";
    
    static const char* fragment_materialDefault = "\
struct Material {\
    float shininess;\
";

    static const char* fragment_material_diffuseTexture = "sampler2D diffuse;";
    static const char* fragment_material_specularTexture = "sampler2D specular;";

    static const char* fragment_materialEnd = "}; uniform Material material;";
    
    // static const char* fragment_diffuseTexture = "uniform sampler2D texture_diffuse";
    
    static const char* fragment_lightDefaultHeader = "\
in vec3 Normal;\
in vec3 FragPos;\
uniform vec3 viewPos;\
";
    
    static const char* fragment_lightDefaultMainStart = "\
vec3 norm = normalize(Normal);\
vec3 viewDir = normalize(viewPos - FragPos);\
";
    
    static const char* fragment_lightDefaultMainEnd = "outColor = vec4(result, 1.0);";
    
    static const char* fragment_directionalLight = "\
struct DirLight {\
    vec3 direction;\
\
    vec3 ambient;\
    vec3 diffuse;\
    vec3 specular;\
};\
\
uniform DirLight dirLight;\
\
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {\
    vec3 lightDir = normalize(-light.direction);\
    // Diffuse shading\n\
    float diff = max(dot(normal, lightDir), 0.0);\
    // Specular shading\n\
    vec3 reflectDir = reflect(-lightDir, normal);\
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\
    // Combine results\n\
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\
    return (ambient + diffuse + specular);\
}\
";
    
    static const char* fragment_directionalLightMain = "vec3 result = CalcDirLight(dirLight, norm, viewDir);";
    
    static const char* fragment_pointLightNumber = "\n#define NR_POINT_LIGHTS ";
    static const char* fragment_pointLightNumberEndKey = "\n";
    
    static const char* fragment_pointLight = "\
struct PointLight {\
    vec3 position;\
    \
    float constant;\
    float linear;\
    float quadratic;\
    \
    vec3 ambient;\
    vec3 diffuse;\
    vec3 specular;\
};\
\
uniform PointLight pointLights[NR_POINT_LIGHTS];\
\
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\
{\
    vec3 lightDir = normalize(light.position - fragPos);\
    // Diffuse shading\n\
    float diff = max(dot(normal, lightDir), 0.0);\
    // Specular shading\n\
    vec3 reflectDir = reflect(-lightDir, normal);\
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\
    // Attenuation\n\
    float distance = length(light.position - fragPos);\
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\
    // Combine results\n\
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\
    ambient *= attenuation;\
    diffuse *= attenuation;\
    specular *= attenuation;\
    return (ambient + diffuse + specular);\
}\
";
    
    static const char* fragment_pointLightMain = "\
for(int i = 0; i < 2; i++)\
    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);\
";
}

#endif