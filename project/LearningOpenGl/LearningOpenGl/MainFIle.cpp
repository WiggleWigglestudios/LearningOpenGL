#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define OGT_VOX_IMPLEMENTATION
#include "ogt_vox.h";
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
#include "Player.h";
#include "Object.h";
#include <vector>

float deltaTime = 0.0f;	
float lastFrame = 0.0f; 
Player player = Player(glm::vec3(0, 0, -3), glm::vec3(0, 0, 1),5.0);

double lastXPosMouse = 0.0f;
double lastYPosMouse = 0.0f;
bool locked = false;


//gets called when window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    //glfwGetWindowSize(window, &width, &height);
  //  std::cout << width << " " << height << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE&&key==GLFW_KEY_ESCAPE)
    {
        if (!locked)
        {
           // std::cout << "close" << std::endl;
            glfwSetWindowShouldClose(window, true);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            locked = false;
        }
    }
    
}

void processInputs(GLFWwindow* window)
{
    player.updateMovement(window, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (locked)
    {
        player.updateLook(deltaTime,glm::vec2(xpos- lastXPosMouse,ypos-lastYPosMouse));
    }
    
    lastXPosMouse = xpos;
    lastYPosMouse = ypos;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        locked = true;
    }
}

void printVec3(glm::vec3 _vector)
{
    std::cout << _vector.x << " " << _vector.y << " " << _vector.z << std::endl;
}
void printVec4(glm::vec4 _vector)
{
    std::cout << _vector.x << " " << _vector.y << " " << _vector.z<<" "<<_vector.w << std::endl;
}

void printMat4(glm::mat4 _matrix )
{
    std::cout << "Matrix" << std::endl;
    printVec4(_matrix[0]);
    printVec4(_matrix[1]);
    printVec4(_matrix[2]);
    printVec4(_matrix[3]);
    std::cout << " " << std::endl;
}




std::string readAllText(const std::string& filePath) {
    std::ifstream inFile(filePath);
    return std::string(std::istreambuf_iterator<char>(inFile), {});
}



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



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);




    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glViewport(0, 0, 800, 600);


    //making it so when window sizes changes framebuffer_size_callback is called
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);



    Shader basicShader("vert.glsl", "frag.glsl");




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



    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int VAO;
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




    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    int width, height, nrChannels;
    unsigned char* imageData = stbi_load("Textures\\Dark_Oak_Log_Comparison.png", &width, &height, &nrChannels, 0);
    if (imageData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(imageData);




    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    imageData = stbi_load("Textures\\sand3.png", &width, &height, &nrChannels, 0);
    if (imageData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(imageData);



    basicShader.use();
    basicShader.setInt("texture1", 0);
    basicShader.setInt("texture2", 1);










    Shader voxelShader = Shader("voxelVert.glsl", "voxelFrag.glsl");


    Object testObject = Object(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), "vox\\generic_sedan_red.vox", 1);
    testObject.updateShader(voxelShader);
    testObject.updateVolumeTexture();
    testObject.updatePaletteTexture();
    testObject.createVertexBufferObject();


    Object testObject2 = Object(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), "vox\\generic_sedan_red.vox", 0);
    testObject2.updateShader(voxelShader);
    testObject2.updateVolumeTexture();
    testObject2.updatePaletteTexture();
    testObject2.createVertexBufferObject();

    Object testObject3 = Object(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), "vox\\tree_birch.vox", 0);
    testObject3.updateShader(voxelShader);
    testObject3.updateVolumeTexture();
    testObject3.updatePaletteTexture();
    testObject3.createVertexBufferObject();

    Object testObject4 = Object(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), "vox\\tree_birch.vox", 1);
    testObject4.updateShader(voxelShader);
    testObject4.updateVolumeTexture();
    testObject4.updatePaletteTexture();
    testObject4.createVertexBufferObject();



   
    
    

    lastFrame = glfwGetTime();
    float rot = 0.0;
    int frameCount = 0;
    float lastFrameFrameCount = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        deltaTime = glfwGetTime() - lastFrame;
        lastFrame = glfwGetTime();

        if (frameCount == 100)
        {
            frameCount = 0;
            std::cout << (100.0/(glfwGetTime()-lastFrameFrameCount)) << std::endl;
            lastFrameFrameCount = glfwGetTime();
        }
        frameCount++;

       // testObject.rotate(2 * deltaTime, glm::vec3(0.0, 1.0, 0.00));
       // testObject.rotate(0.5 * deltaTime, glm::vec3(0.0, 0, 1.0));
        //testObject.pos = glm::vec3(cos(lastFrame), 0.0, sin(lastFrame));

        processInputs(window);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClearColor(0.2f, 0.1f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       // glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);

        float timeValue = glfwGetTime();
        basicShader.use();
        basicShader.setFloat("iTime", timeValue);

        glm::vec3 cubePos = player.pos +(player.lookDir);
       // printVec3(cubePos);
       // printVec3(player.pos);
        glm::mat4 model = glm::translate(glm::mat4(1.0), cubePos) * glm::scale(glm::mat4(1.0),glm::vec3(1.0/16.0, 1.0 / 16.0, 1.0 /16.0));
         model = glm::rotate(model, glm::radians(timeValue * 100), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(timeValue * 30), glm::vec3(0.0f, 1.0f, 0.0f));
        
        
        
        glm::mat4 view = player.generateViewMat();
        
        glfwGetWindowSize(window, &width, &height);
        float fov = glm::radians(60.0f);
        glm::mat4 projection = glm::perspective(fov, (float)width / (float)height, 0.01f, 100.0f);

        basicShader.setMat4("model", model);
        basicShader.setMat4("view", view);
        basicShader.setMat4("projection", projection);



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);



        glfwGetWindowSize(window, &width, &height);
        testObject.voxelShader.use();
        testObject.voxelShader.setVec3("windowSize", float(width), float(height), fov);
        testObject.voxelShader.setVec3("cameraPos", player.pos.x, player.pos.y, player.pos.z);// cubePos.x, cubePos.y, cubePos.z);
        testObject.voxelShader.setVec3("cameraLookDir", player.lookDir.x, player.lookDir.y, player.lookDir.z);
        
        testObject.pos = glm::vec3(0, 0, 0);
        testObject.rotate(3.14159265359/2, glm::normalize(glm::vec3(1, 0, 0)));
        testObject.render(view, projection);
        testObject.rotate(-3.14159265359 / 2, glm::normalize(glm::vec3(1, 0, 0)));
        testObject2.pos = glm::vec3(-1.0, -0.5, 1.6);
        testObject2.rotate(3.14159265359 / 2, glm::normalize(glm::vec3(0, 0, 1)));
        testObject2.render(view, projection);
        testObject2.rotate(-3.14159265359 / 2, glm::normalize(glm::vec3(0, 0, 1)));


        testObject3.pos = glm::vec3(-3.0/8.0, 7.5+1.0/8.0, 10.0+1.0/8.0);
        testObject3.rotate(3.14159265359 / 2, glm::normalize(glm::vec3(1, 0, 0)));
        testObject3.render(view, projection);
        testObject3.rotate(-3.14159265359 / 2, glm::normalize(glm::vec3(1, 0, 0)));

        testObject4.pos = glm::vec3(0.0, 0.0, 10.0);
        testObject4.rotate(3.14159265359 / 2, glm::normalize(glm::vec3(1, 0, 0)));
        testObject4.render(view, projection);
        testObject4.rotate(-3.14159265359 / 2, glm::normalize(glm::vec3(1, 0, 0)));


       // testObject.rotate(0.5*deltaTime, glm::normalize(glm::vec3(0, 1, 0)));
        rot += 1.0 * deltaTime;
       // testObject.rotate(rot, glm::normalize(glm::vec3(0, 1, 0)));
        glm::vec3 spacing = testObject.voxelSize;
        
        spacing.x += 1.0;
        spacing.y += 1.0;
        spacing.z += 1.0;

        spacing.x /= 8.0;
        spacing.y /= 8.0;
        spacing.z /= 8.0;
        for (int xPos = 0; xPos < 11; xPos++)
        {
            for (int yPos = 0; yPos < 1; yPos++)
            {
                for (int zPos = 0; zPos < 11; zPos++)
                {
                   // testObject.pos = glm::vec3(xPos * 4.0, yPos * 4.0 + floor((std::sin(float(xPos) / 3.0) + std::sin(float(zPos) / 3.0)) / 2.0 * 8.0) / 8.0, zPos * 4.0);
                    
                  //  testObject.rotate(rot, glm::normalize(glm::vec3(sin(xPos), 1+cos(yPos), sin(zPos * 4))));
                    //testObject.pos = glm::vec3(xPos * spacing.x, yPos * spacing.y , zPos * spacing.z);
                   // testObject.render(view, projection);
                   // testObject.rotate(-rot, glm::normalize(glm::vec3(sin(xPos), 1 + cos(yPos), sin(zPos * 4))));
                }
            }
        }
        //testObject.rotate(-rot, glm::normalize(glm::vec3(0, 1, 0)));
       // testObject.pos = glm::vec3(1, 0, 0);
       // testObject.rotate(5, glm::normalize(glm::vec3(1, 1, 0)));
       // testObject.render(view, projection);
       // testObject.rotate(-5, glm::normalize(glm::vec3(1, 1, 0)));
       // testObject.pos = glm::vec3(-0.5, 0.5, 0);
      //  testObject.rotate(rot/5.0, glm::normalize(glm::vec3(1, 1, 0)));
      //  testObject.render(view, projection);
       // testObject.rotate(-rot/5.0, glm::normalize(glm::vec3(1, 1, 0)));


        glfwSwapBuffers(window);
        glfwPollEvents();



    }

    glfwTerminate();
    return 0;

}