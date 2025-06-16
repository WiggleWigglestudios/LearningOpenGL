#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>



class Object
{
public:
	glm::vec3 pos;
	glm::vec3 forwardDir;
	glm::vec3 upDir;
	glm::i8vec3 voxelSize;
	unsigned char voxelData[1];
	unsigned char voxelPalatte[256*3];

	Object(glm::vec3 spawnPos, glm::vec3 spawnForwardDir,glm::vec3 spawnUpDir, unsigned char _voxelData[], unsigned char voxelPalatte[]);

	void render();


};
