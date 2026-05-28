#include "structs/global-state.h"
#include "navigation.h"
#include "calculate-ratio-letterbox.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    global_state* state = (global_state*) glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS) {
        const char* next_path = NULL;

        if (key == GLFW_KEY_RIGHT) {
            next_path = nav_next_image(&state->nav);
        }
        else if (key == GLFW_KEY_LEFT) {
            next_path = nav_prev_image(&state->nav);
        }
        else if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        // if path change, should reload the texture
        if (next_path) {
            if (state->image.id != 0) {
                glDeleteTextures(1, &state->image.id);
            }
            state->image = rn_load_texture(next_path);
            state->zoom = 1.0f;
            calculate_ratio_letterbox(window);
        }
    }
}