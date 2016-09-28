#ifndef TERRAINS_TERRAIN_HPP
#define TERRAINS_TERRAIN_HPP

#include <Mesh.hpp>
#include <Shader/ShaderProgram.hpp>

#include <Camera.hpp>

namespace Terrains {
    class Terrain {
    public:
        static constexpr float SIZE = 800.f;
        static const int VERTEX_COUNT = 128;
        
    public:
        Terrain(int gridX, int gridZ) {
            Shader::Settings shaderSettings;
            shaderSettings.fragment.light.directionalLight = true;
            shaderSettings.fragment.light.pointLights = 2;
            
            m_shader.loadShader(std::string("res/terrain.vert"), GL_VERTEX_SHADER);
            m_shader.loadShader(std::string("res/terrain.frag"), GL_FRAGMENT_SHADER);
            Shader::initializeShader(m_shader, shaderSettings);
            
            assert(!checkErrors());
            
            m_mesh = generateTerrain();
            //m_mesh.addTexture(Loader::loadTexture("res/grass.png", GL_REPEAT, GL_REPEAT), TextureType::Diffuse);
            
            std::cout << "Terrain out!\n";
        };
        
        Shader::ShaderProgram& getShader() { return m_shader; }
        
        void setProjectionMatrix(const glm::mat4& projectionMatrix) { 
            m_shader.start();
            
            m_shader.loadMatrix(Shader::Uniform::PROJECTION_MATRIX, projectionMatrix);
            
            glm::mat4 modelm;
            modelm = glm::translate(modelm, glm::vec3(0.f, 0.f, 0.f));
            m_shader.loadMatrix(Shader::Uniform::MODEL_MATRIX, modelm);
            
            m_shader.stop();
        }
        
        void draw() {
            m_mesh.draw(m_shader);
        }
        
        Mesh generateTerrain() {
            int count = VERTEX_COUNT * VERTEX_COUNT;
            Mesh mesh;
            mesh.setVerticesCount(count);
            mesh.setIndicesCount(6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1));
    		for(int i=0;i<VERTEX_COUNT;i++){
    			for(int j=0;j<VERTEX_COUNT;j++){
                    Vertex vertex;
                    vertex.Position = {(float)j/((float)VERTEX_COUNT - 1) * SIZE
                                      ,0
                                      ,(float)i/((float)VERTEX_COUNT - 1) * SIZE
                    };
                    vertex.Normal = {0,1,0};
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
        Mesh m_mesh;
        Shader::ShaderProgram m_shader;
    };
}

#endif