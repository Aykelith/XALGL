#version 400 core

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  

out vec4 outColor;

// #define NR_POINT_LIGHTS 2
// 
// struct DirLight {
//     vec3 direction;
//   
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };
// 
// struct PointLight {    
//     vec3 position;
//     
//     float constant;
//     float linear;
//     float quadratic;  
// 
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

uniform sampler2D blackTexture;
uniform sampler2D redTexture;
uniform sampler2D greenTexture;
uniform sampler2D blueTexture;
uniform sampler2D blendmapTexture;

// uniform vec3 viewPos;
// uniform DirLight dirLight;
// uniform PointLight pointLights[NR_POINT_LIGHTS];
// uniform uint pointLightsCount;
// 
// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 

void main()
{
    // // Properties
    // //vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    // vec3 norm = normalize(Normal);
    // vec3 viewDir = normalize(viewPos - FragPos);
    // 
    // // Phase 1: Directional lighting
    // vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // // Phase 2: Point lights
    // for(int i = 0; i < 2; i++)
    //     result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // // Phase 3: Spot light
    // //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    vec4 blendmapColor = texture(blendmapTexture, TexCoords);
    
    float backTextureAmount = 1 - (blendmapColor.r + blendmapColor.g + blendmapColor.b);
    vec2 tiledCoords = TexCoords * 40.f;
    vec4 blackTextureColor = texture(blackTexture, tiledCoords) * backTextureAmount;
    vec4 redTextureColor = texture(redTexture, tiledCoords) * blendmapColor.r;
    vec4 greenTextureColor = texture(greenTexture, tiledCoords) * blendmapColor.g;
    vec4 blueTextureColor = texture(blueTexture, tiledCoords) * blendmapColor.b;
    
    vec4 totalColor = blackTextureColor + redTextureColor + greenTextureColor + blueTextureColor;
    
    outColor = totalColor;
}

// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
// {
//     vec3 lightDir = normalize(-light.direction);
//     // Diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // Specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // Combine results
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(1.f, 1.f, 1.f);
//     return (ambient + diffuse + specular);
// }
// 
// // Calculates the color when using a point light.
// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // Diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // Specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // Attenuation
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//     // Combine results
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(1.f, 1.f, 1.f);
//     ambient *= attenuation;
//     diffuse *= attenuation;
//     specular *= attenuation;
//     return (ambient + diffuse + specular);
// }