#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "../include/glad/glad.h"
#include "../include/error-callback.h"
#include "../include/color.h"
#include "../include/runara/include/runara/runara.h"

#define BACKGROUND_COLOR 0.3f, 0.1f, 0.4f, 1.0f
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

typedef struct {
    RnState* render;

} global_state;

static global_state s;
void resize_callback(GLFWwindow* window, int width, int height) {
    rn_resize_display(s.render, width, height);
}

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

    glfwSetFramebufferSizeCallback(window, resize_callback);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Error initializing GLAD\n");
        return -1;
    }
    s.render = rn_init(SCREEN_WIDTH, SCREEN_HEIGHT, (RnGLLoader)glfwGetProcAddress);

    RnTexture image = rn_load_texture("./gojo.jpg");

    while (!glfwWindowShouldClose(window)) {
        //This function set the color of the screen but this doesn't paint the screen
        glClearColor(BACKGROUND_COLOR);
        //This is the function that paints the screen with the color set by the glClearColor 
        glClear(GL_COLOR_BUFFER_BIT);

        //rn_clear_color(rn_color_from_hex(0x191970ff));

        rn_begin(s.render);
        
        float best_image_ratio = MIN
        (
            s.render->render_w / (float)image.width, 
            s.render->render_h / (float)image.height
        );

        vec2s image_size = (vec2s){image.width * best_image_ratio, image.height * best_image_ratio};
        vec2s image_position = (vec2s){(s.render->render_w - image_size.x) /2.0f, (s.render->render_h - image_size.y) /2.0f};
        
        
        rn_image_render(s.render, image_position, RN_WHITE, (RnTexture){
            .width = (uint32_t)image_size.x,
            .height = (uint32_t)image_size.y,
            .id = image.id
        });
        rn_end(s.render);

        glfwSwapBuffers(window);
        glfwWaitEvents();  
    }

    rn_terminate(s.render);  // Clean
    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Window Closed");
    return 0;
}