#include "structs/global-state.h"

void scroll_callback(GLFWwindow* window, double delta_x, double delta_y){
    global_state* state = (global_state*)glfwGetWindowUserPointer(window);

    float old_zoom = state->zoom;
    state->zoom += delta_y;
    state->zoom = MIN(state->zoom,15.0);
    state->zoom = MAX(state->zoom, 1.0f);

    float zoom_factor = state->zoom / old_zoom;
    double cursor_x_position, cursor_y_position;
    //Get the cursor position
    glfwGetCursorPos(window, &cursor_x_position, &cursor_y_position);

    state->image_position.x = (state->image_size.x * state->zoom > state->letterbox.x)? glm_clamp(cursor_x_position - (cursor_x_position - state->image_position.x) * zoom_factor, state->letterbox.x - (state->image_size.x * state->zoom), 0.0) :
    (state->letterbox.x - (state->image_size.x * state->zoom)) /2.0f;


    state->image_position.y = (state->image_size.y * state->zoom > state->letterbox.y)? glm_clamp(cursor_y_position - (cursor_y_position - state->image_position.y) * zoom_factor, state->letterbox.y - (state->image_size.y * state->zoom), 0.0) :
    (state->letterbox.y - (state->image_size.y * state->zoom)) /2.0f;
}