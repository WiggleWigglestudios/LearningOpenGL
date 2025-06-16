#include "Object.h"


Object::Object(glm::vec3 spawnPos, glm::vec3 spawnForwardDir, glm::vec3 spawnUpDir, std::vector<unsigned char> _voxelData,
	glm::i8vec3 _voxelSize, std::vector<unsigned char> _voxelPalatte)
{
	pos = spawnPos;
	forwardDir = spawnForwardDir;
	upDir = spawnUpDir;
	voxelData.resize(_voxelData.size());
	for(int i=0;i<voxelData.size();i++)
	{ 
		voxelData[i] = _voxelData[i];
	}
	voxelPalatte.resize(_voxelPalatte.size());
	for (int i = 0; i < voxelPalatte.size(); i++)
	{
		voxelPalatte[i] = _voxelPalatte[i];
	}
	voxelSize = _voxelSize;
}

void Object::updateVolumeTexture()
{
	glGenTextures(1, &volumeTexture);
	glBindTexture(GL_TEXTURE_2D, volumeTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	if (voxelData.size() > 0)
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R, voxelSize.x, voxelSize.y, voxelSize.z, 0, GL_R, GL_UNSIGNED_BYTE, voxelData.data());
	}
	else
	{
		std::cout << "no voxel data. can't update volume texture" << std::endl;
	}

}

void Object::createVertexBufferObject()
{
    float vertices[] = {
        //bottom face
        -0.5f, -0.5f, -0.5f,    0.0f,0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f,0.5f,
        0.5f, -0.5f, 0.5f,      0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,     0.5f, 0.0f,

        //top face
        -0.5f, 0.5f, 0.5f,     0.0f,0.0f,
        -0.5f, 0.5f, -0.5f,    0.0f,0.5f,
        0.5f, 0.5f, -0.5f,     0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,      0.5f, 0.0f,

        //back face
        -0.5f, 0.5f, -0.5f,     0.0f,0.5f,
        -0.5f, -0.5f, -0.5f,    0.0f,1.0f,
        0.5f, -0.5f, -0.5f,     0.5f, 1.0f,
        0.5f, 0.5f, -0.5f,      0.5f, 0.5f,

        //front face
        -0.5f, -0.5f, 0.5f,   0.0f,0.5f,
        -0.5f, 0.5f, 0.5f,    0.0f,1.0f,
        0.5f, 0.5f, 0.5f,     0.5f, 1.0f,
        0.5f, -0.5f, 0.5f,    0.5f, 0.5f,

        //left face
        -0.5f, -0.5f, -0.5f,   0.0f,0.5f,
        -0.5f, 0.5f, -0.5f,    0.0f,1.0f,
        -0.5f, 0.5f, 0.5f,     0.5f, 1.0f,
        -0.5f, -0.5f, 0.5f,    0.5f, 0.5f,

        //right face
        0.5f, -0.5f, 0.5f,   0.0f,0.5f,
        0.5f, 0.5f, 0.5f,    0.0f,1.0f,
        0.5f, 0.5f, -0.5f,   0.5f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.5f, 0.5f,
    };
    unsigned int indices[] = {
        //bottom face
        0, 1, 2,
        0, 2, 3,
        //top face
        4, 5, 6,
        4, 6, 7,
        //back face
        8, 9, 10,
        8, 10, 11,
        //front face
        12, 13, 14,
        12, 14, 15,
        //left face
        16, 17, 18,
        16, 18, 19,
        //right face
        20, 21, 22,
        20, 22, 23
    };

	glGenBuffers(1, &VBO);

	
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//first parameter tells which vertex attribute we are configuring. In our case vertex 
   //attribute 0 is a vec3 position (defined in the ver.glsl shader)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Object::updateShader(Shader inputShader)
{
    voxelShader = inputShader;
}

void Object::render(glm::mat4 viewMat,glm::mat4 projectionMat)
{

    voxelShader.use();
    voxelShader.setInt("texture1", 0);
    voxelShader.setInt("texture2", 1);

    glm::mat4 modelMat = glm::lookAt(pos, pos + forwardDir, upDir);
    modelMat = glm::translate(modelMat, pos);

    voxelShader.setMat4("model", modelMat);
    voxelShader.setMat4("view", viewMat);
    voxelShader.setMat4("projection", projectionMat);
    /*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);*/

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


