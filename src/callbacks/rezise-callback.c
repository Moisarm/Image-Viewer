#include "../../include/structs/global-state.h" //global state struct
#include <GLFW/glfw3.h>
#include "../../include/calculate-ratio-letterbox.h"


void resize_callback(GLFWwindow* window, int width, int height) {
    // Get the pointer to the global_state from the window
    global_state* state = (global_state*)glfwGetWindowUserPointer(window);
    if (!state) return;

    // Execute the rezise in the graphic engine
    rn_resize_display(state->render, width, height);
    
    // Recalculate the available area minus the title bar size
    state->letterbox = (vec2s){ (float)width, (float)height - state->title_bar_size };

   calculate_ratio_letterbox(window);
}