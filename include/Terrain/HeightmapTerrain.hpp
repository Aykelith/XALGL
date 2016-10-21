#ifndef TERRAINS_HEIGHTMAP_HPP
#define TERRAINS_HEIGHTMAP_HPP

namespace Terrains {
    class HeightmapTerrain {
    public:
        static constexpr float SIZE = 800.f;
        static constexpr float MAX_HEIGHT = 60.f;
        static constexpr float MIN_HEIGHT = 0.f;
        
    public:
        HeightmapTerrain() = default;
        
        void loadHeightmap(int gridX, int gridZ, const std::string& file) {
            Shader::Settings shaderSettings;
            shaderSettings.fragment.light.directionalLight = true;
            shaderSettings.fragment.light.pointLights = 2;
            
            m_shader.loadShader(std::string("res/terrain.vert"), GL_VERTEX_SHADER);
            m_shader.loadShader(std::string("res/terrain.frag"), GL_FRAGMENT_SHADER);
            Shader::initializeShader(m_shader, shaderSettings);
            
            assert(!checkErrors());
            
            m_mesh = generateTerrain(file);
        }
        
        Shader::ShaderProgram& getShader() { return m_shader; }
        
        void setProjectionMatrix(const glm::mat4& projectionMatrix) { 
            m_shader.start();
            
            m_shader.loadMatrix(static_cast<int>(Shader::Uniform::PROJECTION_MATRIX), projectionMatrix);
            
            glm::mat4 modelm;
            modelm = glm::translate(modelm, glm::vec3(0.f, 0.f, 0.f));
            m_shader.loadMatrix(static_cast<int>(Shader::Uniform::MODEL_MATRIX), modelm);
            
            m_shader.stop();
        }
        
        void draw() {
            m_mesh.draw(m_shader);
        }
        
        Mesh generateTerrain(const std::string& file) {
            sf::Image texture;
            texture.loadFromFile(file);
            
            int VERTEX_COUNT = texture.getSize().y;
            
            int count = VERTEX_COUNT * VERTEX_COUNT;
            Mesh mesh;
            mesh.setVerticesCount(count);
            mesh.setIndicesCount(6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1));
    		for(int i=0;i<VERTEX_COUNT;i++) {
    			for(int j=0;j<VERTEX_COUNT;j++) {
                    Vertex vertex;
                    vertex.Position = {(float)j/((float)VERTEX_COUNT - 1) * SIZE
                                      ,getHeight(i,j, texture)
                                      ,(float)i/((float)VERTEX_COUNT - 1) * SIZE
                    };
                    vertex.Normal = calculateNormal(i,j, texture);
                    vertex.TexCoords = {(float)j/((float)VERTEX_COUNT - 1), (float)i/((float)VERTEX_COUNT - 1)};
                              
    				mesh.addVertex(vertex);
    			}
    		}
            
    		for(int gz=0;gz<VERTEX_COUNT-1;gz++){
    			for(int gx=0;gx<VERTEX_COUNT-1;gx++){
    				int topLeft = (gz*VERTEX_COUNT)+gx;
    				int topRight = topLeft + 1;
    				int bottomLeft = ((gz+1)*VERTEX_COUNT)+gx;
    				int bottomRight = bottomLeft + 1;
    				mesh.addIndex(topLeft);
    				mesh.addIndex(bottomLeft);
    				mesh.addIndex(topRight);
    				mesh.addIndex(topRight);
    				mesh.addIndex(bottomLeft);
    				mesh.addIndex(bottomRight);
    			}
    		}
            
            mesh.glue();
    		return mesh;
        }
        
    private:
        float getHeight(uint x, uint z, sf::Image& texture) {
            if (x < 0 || x > texture.getSize().x || z < 0 || z > texture.getSize().y) return 0;
            
            float c = float(texture.getPixel(x,z).r);
            return ((MAX_HEIGHT - MIN_HEIGHT)*c)/255.f + MIN_HEIGHT;
        }
        
        glm::vec3 calculateNormal(uint x, uint z, sf::Image& texture) {
            float heightL = getHeight(x-1,z, texture);
            float heightR = getHeight(x+1, z, texture);
            float heightD = getHeight(x, z-1, texture);
            float heightU = getHeight(x, z+1, texture);
            glm::vec3 normal = glm::vec3(heightL-heightR, 2.f, heightD - heightU);
            return glm::normalize(normal);
        }
        
    private:
        Mesh m_mesh;
        Shader::ShaderProgram m_shader;
    };
}

#endif