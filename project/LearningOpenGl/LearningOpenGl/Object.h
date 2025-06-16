#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "Shader.h"


class Object
{
public:
	glm::vec3 pos;
	glm::vec3 forwardDir;
	glm::vec3 upDir;
	glm::i8vec3 voxelSize;
	std::vector<unsigned char> voxelData;
	std::vector<unsigned char> voxelPalatte; //256 colors 8r 8b 8g 2a 2emission 2metalic 2 roughness (4bytes in total)


	unsigned int VBO; //vertex buffer object
	unsigned int EBO; //index buffer
	unsigned int VAO; //vertex array object 

	Shader voxelShader;

	unsigned int volumeTexture;

	Object(glm::vec3 spawnPos, glm::vec3 spawnForwardDir, glm::vec3 spawnUpDir, std::vector<unsigned char> _voxelData,
		glm::i8vec3 _voxelSize, std::vector<unsigned char> _voxelPalatte);

	void updateShader(Shader inputShader);

	void render(glm::mat4 viewMat, glm::mat4 projectionMat);

	void createVertexBufferObject();

	void updateVolumeTexture();

};
