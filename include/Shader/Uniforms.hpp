#ifndef SHADER_UNIFORMS_HPP
#define SHADER_UNIFORMS_HPP

namespace Shader {
    namespace Impl {
        static const int MAX_MATERIALS = 10;
        static const int MAX_MATERIAL_DIFFUSE_TEXTURES = 10;
        
        static const int BASE_COUNT = 6 + MAX_MATERIALS + MAX_MATERIAL_DIFFUSE_TEXTURES;
        static const int BLENDMAP_COUNT = 5;
        static const int TEXT_COUNT = 2; 
        
        static const int BASE_START = 0;
        static const int BLENDMAP_START = BASE_START + BASE_COUNT;
        static const int TEXT_START = BLENDMAP_START + BLENDMAP_COUNT;
    }
    
    static const int NEXT_UNIFORM = Impl::TEXT_START + Impl::TEXT_COUNT;
}

#endif