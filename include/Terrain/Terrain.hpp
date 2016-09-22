#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <Mesh.hpp>
#include <Shader/ShaderProgram.hpp>

class Terrain {
public:
    static constexpr float SIZE = 800.f;
    static const int VERTEX_COUNT = 128;
    
public:
    Terrain(int gridX, int gridZ, const glm::mat4& projectionMatrix)
        : m_x { gridX * SIZE }
        , m_z { gridZ * SIZE}
    {
        Shader::Settings shaderSettings;
        shaderSettings.fragment.material = true;
        shaderSettings.fragment.light.directionalLight = true;
        shaderSettings.fragment.light.pointLights = 2;
        shaderSettings.fragment.diffuseTextures = 1;
        
        m_shader.loadShader(std::string("res/terrain.vert"), GL_VERTEX_SHADER);
        m_shader.loadShader(std::string("res/terrain.frag"), GL_FRAGMENT_SHADER);
        Shader::initializeShader(m_shader, shaderSettings);
        m_shader.start();
        m_shader.loadMatrix(Shader::Uniform::PROJECTION_MATRIX, projectionMatrix);
        m_shader.stop();
        
        assert(!checkErrors());
        
        m_mesh = generateTerrain();
    };
    
    void draw() {
        m_shader.start();
        m_mesh.draw(m_shader);
        m_shader.stop();
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
    float m_x;
    float m_z;
    Mesh m_mesh;
    Shader::ShaderProgram m_shader;
};

#endif