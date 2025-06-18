#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>







class Player
{
public:
	glm::vec3 pos;
	glm::vec3 lookDir;
	float movementSpeed;
	float mouseSensitivity;
	Player(glm::vec3 spawnPos, glm::vec3 spawnLookDir);
	Player(glm::vec3 spawnPos, glm::vec3 spawnLookDir,float _movementSpeed);

	void updateMovement(GLFWwindow* window, float deltaTime);
	void updateLook(float deltaTime,glm::vec2 deltaMouse);
	glm::mat4 generateViewMat();
};
