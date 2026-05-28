#include "../include/structs/global-state.h"

void calculate_ratio_letterbox(GLFWwindow* window){
    global_state* state = (global_state*)glfwGetWindowUserPointer(window);

    if(!state) return;

    float best_image_ratio = MIN
    (
        state->letterbox.x / (float)state->image.width, 
        state->letterbox.y / (float)state->image.height
    );

    state->image_size = (vec2s){state->image.width * best_image_ratio, state->image.height * best_image_ratio};
    state->image_position = (vec2s){(state->letterbox.x - state->image_size.x) /2.0f, (state->letterbox.y - state->image_size.y) /2.0f};
}