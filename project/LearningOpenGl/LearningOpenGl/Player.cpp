#include "Player.h"

Player::Player(glm::vec3 spawnPos, glm::vec3 spawnLookDir)
{
    pos = spawnPos;
    lookDir = normalize(spawnLookDir);
    movementSpeed = 5.0;
    mouseSensitivity = 4;
}
Player::Player(glm::vec3 spawnPos, glm::vec3 spawnLookDir,float _movementSpeed)
{
    pos = spawnPos;
    lookDir = spawnLookDir;
    movementSpeed = _movementSpeed;
    mouseSensitivity = 4;
}

void Player::updateMovement(GLFWwindow* window,float deltaTime)
{
    //std::cout << "balls" << std::endl;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
       //  std::cout << "w " << lookDir.x << " "<< lookDir.y << " "<< lookDir.z<< " " << std::endl;
        pos += movementSpeed * lookDir*deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        pos -= movementSpeed * lookDir * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        pos -= glm::normalize(glm::cross(lookDir, glm::vec3(0,1,0))) * movementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        pos += glm::normalize(glm::cross(lookDir, glm::vec3(0, 1, 0))) * movementSpeed * deltaTime;
    }

}
void Player::updateLook(float deltaTime, glm::vec2 deltaMouse)
{
    lookDir = glm::vec3(glm::vec4(lookDir, 1.0) * glm::rotate(glm::mat4(1.0), deltaMouse.x * deltaTime * mouseSensitivity, glm::vec3(0, 1, 0)));
    lookDir = glm::vec3(glm::vec4(lookDir, 1.0) * glm::rotate(glm::mat4(1.0), deltaMouse.y * deltaTime * mouseSensitivity, glm::normalize(glm::cross(lookDir, glm::vec3(0, 1, 0)))));
}





