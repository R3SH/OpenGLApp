#pragma once

#include <vector>

#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "Shader.h"
//#include "../Texture.h"
//#include "../VertexBuffer.h"
//#include "../VertexBufferLayout.h"

#include "../Camera.h"


using namespace std;

#define MAX_BONE_INFLUENCE 4

struct MeshVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct MeshTexture {
	unsigned int id;
	string type;
    string path;
};

class Mesh {
public:
    // mesh data
    vector<MeshVertex>       vertices;
    vector<unsigned int> indices;
    vector<MeshTexture>      textures;
    unsigned int VAO;


    Mesh(vector<MeshVertex> vertices, vector<unsigned int> indices, vector<MeshTexture> textures);
    void Draw(Shader& shader);
private:
    //  render data
    unsigned int VBO, EBO;

    void setupMesh();
};
