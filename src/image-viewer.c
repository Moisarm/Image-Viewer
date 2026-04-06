#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "../include/glad/glad.h"
#include "../include/error-callback.h"
#include "../include/color.h"
#include "../include/runara/include/runara/runara.h"

#define BACKGROUND_COLOR 0.3f, 0.1f, 0.4f, 1.0f

int main() {
    const unsigned int SCREEN_WIDTH = 800;
    const unsigned int SCREEN_HEIGHT = 800;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Image Viewer", NULL, NULL);
 
    if (!window) {
        printf("Error creating the window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    RnState* state = rn_init(SCREEN_WIDTH, SCREEN_HEIGHT, (RnGLLoader)glfwGetProcAddress);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Error initializing GLAD\n");
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        //This function set the color of the screen but this doesn't paint the screen
        glClearColor(BACKGROUND_COLOR);
        //This is the function that paints the screen with the color set by the glClearColor 
        glClear(GL_COLOR_BUFFER_BIT);

        rn_begin(state);
        rn_rect_render(state, (vec2s){50.0f, 50.0f}, 
                              (vec2s){50.0f, 50.0f}, RN_RED);
        rn_end(state);

        glfwSwapBuffers(window);
        glfwWaitEvents();  
    }

    rn_terminate(state);  // Clean
    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Window Closed");
    return 0;
}