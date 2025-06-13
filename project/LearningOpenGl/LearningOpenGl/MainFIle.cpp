#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <iterator>
#include <fstream>


//gets called when window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


std::string readAllText(const std::string& filePath) {
    std::ifstream inFile(filePath);
    return std::string(std::istreambuf_iterator<char>(inFile), {});
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





    /**
    * GO BACK TO SECTION 6.5 TO MAKE SHADER CLASS THEN GO THROUGH SECTION 7 FOR LOADING TEXTURES
    */



    std::string vertexShaderText = readAllText("vert.glsl");

    const char* vertexShaderSource = (vertexShaderText.c_str()); 



    unsigned int vertexShader;
    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }



    std::string fragShaderText = readAllText("frag.glsl");

    const char* fragmentShaderSource = (fragShaderText.c_str());

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


 
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }



    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();


    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glUseProgram(shaderProgram);

   


    float vertices[] = {
        0.5f, 0.5f, 0.0f,1.0f,0.0f,0.0f, // top right
        0.5f, -0.5f, 0.0f,1.0f,1.0f,0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,1.0f,0.0f,1.0f, // bottom left
        -0.5f, 0.5f, 0.0f,0.0f,0.0f,0.0f, // top left
        0.0f,0.75f,0.0f,0.0f,0.0f,1.0f//middle top
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
        0,3,4
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);




    while (!glfwWindowShouldClose(window))
    {
        processInput(window);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float timeValue = glfwGetTime();
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "iTime");
        glUseProgram(shaderProgram);
        glUniform1f(vertexColorLocation, timeValue);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;

}