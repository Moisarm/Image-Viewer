#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "../include/glad/glad.h"
#include "../include/error-callback.h"

int main(){
    
    const char* vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";
    //Function that sets error callback
    glfwSetErrorCallback(error_callback);
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Image Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*Initialize glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Error initializing GLAD\n");
        return -1;
    }

    GLfloat vertices[] =
    {
        -0.5f, -0.5f * (float)sqrt(3)/ 3, -0.0f,
        0.5f, -0.5f * (float)sqrt(3)/ 3, 0.0f,
        0.0f, 0.5f * (float)sqrt(3) *2 / 3, 0.0f,
    };

    GLuint vertex_shaders = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shaders,1,&vertex_shader_source,NULL);
    glCompileShader(vertex_shaders);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragment_shader);

    GLuint shader_progam = glCreateProgram();
    glAttachShader(shader_progam, vertex_shaders);
    glAttachShader(shader_progam, fragment_shader);

    glLinkProgram(shader_progam);

    glDeleteShader(vertex_shaders);
    glDeleteShader(fragment_shader);

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);//VAO must be generated before the VBO this is super important
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);     
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //This is a way to comunicate with the vertex shader from the outside
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // glClear(GL_COLOR_BUFFER_BIT);
        
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_progam);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwWaitEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_progam);

    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Window Closed");
    return 0;
}