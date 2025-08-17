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
#include <thread>
#include "Object.h"


class Scene 
{
public:
	std::vector<Object> objects;
	std::string sceneFilePath;
	std::string sceneText;
	Shader voxelShader;


	unsigned int meshVBO; //vertex buffer object for the cube mesh
	unsigned int meshVAO; //vertex array object for the cube mesh

	Scene(const std::string& _sceneFilePath);
	void Load();
	void Render(glm::mat4 viewMat, glm::mat4 projectionMat, glm::vec3 playerPos, glm::vec3 playerLookDir, glm::vec4 windowInfo);
	void setUpInstanceRender();
	void InstanceRender(glm::mat4 viewMat, glm::mat4 projectionMat, glm::vec3 playerPos, glm::vec3 playerLookDir, glm::vec4 windowInfo);
};