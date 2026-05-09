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
    float zoom;
    vec2s image_position, image_size, cull_position;
    RnTexture image;
} global_state;

static global_state state;

void calculate_ratio_letterbox(){
    float best_image_ratio = MIN
    (
        state.render->render_w / (float)state.image.width, 
        state.render->render_h / (float)state.image.height
    );

    state.image_size = (vec2s){state.image.width * best_image_ratio, state.image.height * best_image_ratio};
    state.image_position = (vec2s){(state.render->render_w - state.image_size.x) /2.0f, (state.render->render_h - state.image_size.y) /2.0f};
}

void resize_callback(GLFWwindow* window, int width, int height) {
    rn_resize_display(state.render, width, height);
    calculate_ratio_letterbox();
}

void scroll_callback(GLFWwindow* window, double delta_x, double delta_y){
    float old_zoom = state.zoom;
    state.zoom += delta_y;
    state.zoom = MIN(MAX(state.zoom,1.0), 15.0f);

    float zoom_factor = state.zoom / old_zoom;
    double cursor_x_position, cursor_y_position;
    //Get the cursor position
    glfwGetCursorPos(window, &cursor_x_position, &cursor_y_position);

    state.image_position.x = cursor_x_position - (cursor_x_position - state.image_position.x) * zoom_factor;
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
    glfwSetScrollCallback(window, scroll_callback);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Error initializing GLAD\n");
        return -1;
    }

    state.render = rn_init(SCREEN_WIDTH, SCREEN_HEIGHT, (RnGLLoader)glfwGetProcAddress);
    state.zoom = 1.0f;


    state.image = rn_load_texture("./gojo.jpg");
    calculate_ratio_letterbox();

    while (!glfwWindowShouldClose(window)) {
        //This function set the color of the screen but this doesn't paint the screen
        glClearColor(BACKGROUND_COLOR);
        //This is the function that paints the screen with the color set by the glClearColor 
        glClear(GL_COLOR_BUFFER_BIT);

        //rn_clear_color(rn_color_from_hex(0x191970ff));

        rn_begin(state.render);
        
         state.cull_position = (vec2s){(state.render->render_w - state.image_size.x) /2.0f, (state.render->render_h - state.image_size.y) /2.0f};
        
        rn_set_cull_start_x(state.render, state.cull_position.x);
        rn_set_cull_end_x(state.render, state.cull_position.x + state.image_size.x);

        rn_set_cull_start_y(state.render, state.cull_position.y);
        rn_set_cull_end_y(state.render, state.cull_position.y + state.image_size.y);

        rn_image_render(state.render, state.image_position, RN_WHITE, (RnTexture){
            .width = (uint32_t)state.image_size.x * state.zoom,
            .height = (uint32_t)state.image_size.y * state.zoom,
            .id = state.image.id
        });
        
        rn_unset_cull_start_x(state.render);
        rn_unset_cull_start_y(state.render);
        rn_unset_cull_end_x(state.render);
        rn_unset_cull_end_y(state.render);

        rn_end(state.render);

        glfwSwapBuffers(window);
        glfwWaitEvents();  
    }

    rn_terminate(state.render);  // Clean
    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Window Closed");
    return 0;
}