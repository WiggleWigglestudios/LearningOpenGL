#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <iterator>
#include "Shader.h"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


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
	unsigned int paletteTexture;

	uint8_t* loadFileBytes(const std::string& filename, std::size_t& outSize) {
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file) {
			std::cerr << "Failed to open file: " << filename << '\n';
			outSize = 0;
			return nullptr;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		uint8_t* buffer = new uint8_t[size];
		if (!file.read(reinterpret_cast<char*>(buffer), size)) {
			std::cerr << "Failed to read file: " << filename << '\n';
			delete[] buffer;
			outSize = 0;
			return nullptr;
		}

		outSize = static_cast<std::size_t>(size);
		return buffer;
	}

	Object(glm::vec3 spawnPos, glm::vec3 spawnForwardDir, glm::vec3 spawnUpDir, std::vector<unsigned char> _voxelData,
		glm::i8vec3 _voxelSize, std::vector<unsigned char> _voxelPalatte);

	Object(glm::vec3 spawnPos, glm::vec3 spawnForwardDir, glm::vec3 spawnUpDir, std::string fileName,int modelNumber);

	void updateShader(Shader inputShader);

	void render(glm::mat4 viewMat, glm::mat4 projectionMat);

	void createVertexBufferObject();

	void updateVolumeTexture();

	void updatePaletteTexture();

	void rotate(float angle, glm::vec3 axis);

	glm::mat4 generateRotationMatrix();

	glm::mat4 generateTranslationMatrix();
};
