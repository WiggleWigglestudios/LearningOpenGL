#include "Player.h"

Player::Player(glm::vec3 spawnPos, glm::vec3 spawnLookDir)
{
    pos = spawnPos;
    lookDir = normalize(spawnLookDir);
    movementSpeed = 5.0;
    mouseSensitivity = 1;
}
Player::Player(glm::vec3 spawnPos, glm::vec3 spawnLookDir,float _movementSpeed)
{
    pos = spawnPos;
    lookDir = spawnLookDir;
    movementSpeed = _movementSpeed;
    mouseSensitivity = 1;
}

void Player::updateMovement(GLFWwindow* window,float deltaTime)
{
    glm::vec3 forwardMovementDir = glm::normalize(glm::vec3(lookDir.x, 0, lookDir.z));
    glm::vec3 rightMovementDir = glm::normalize(glm::cross(forwardMovementDir,glm::vec3(0,1,0)));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        pos += movementSpeed * forwardMovementDir *deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        pos -= movementSpeed * forwardMovementDir * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        pos -= rightMovementDir * movementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        pos += rightMovementDir * movementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        pos += glm::vec3(0, 1, 0) * movementSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS|| glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        pos += glm::vec3(0, -1, 0) * movementSpeed * deltaTime;
    }

}

float max(float a, float b)
{
   return a > b ? a : b;
}
float min(float a, float b)
{
    return a<b ? a : b;
}

void Player::updateLook(float deltaTime, glm::vec2 deltaMouse)
{
    float pitch =asin(lookDir.y);
    float yChange = -deltaMouse.y * deltaTime * mouseSensitivity;
    yChange = -pitch + max(min(pitch + yChange, 1.55334303427), -1.55334303427);
    lookDir = glm::vec3(glm::vec4(lookDir, 1.0) * glm::rotate(glm::mat4(1.0),deltaMouse.x * deltaTime * mouseSensitivity, glm::vec3(0, 1, 0)));
    lookDir = glm::vec3(glm::vec4(lookDir, 1.0) * glm::rotate(glm::mat4(1.0), -yChange, glm::normalize(glm::cross(lookDir, glm::vec3(0, 1, 0)))));
    
}

glm::mat4 Player::generateViewMat()
{
    return glm::lookAt(pos, pos + lookDir, glm::vec3(0, 1, 0));;
}



