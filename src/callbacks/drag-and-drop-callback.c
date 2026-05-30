#include "structs/global-state.h"
#include "calculate-ratio-letterbox.h"
#include "navigation.h"

void drag_and_drop_callback(GLFWwindow* window, int count, const char** paths)
{
    if(count <= 0 || !paths) return;

    global_state* state = (global_state*) glfwGetWindowUserPointer(window);

    if(!state) return;

    const char* dropped_file_path = paths[0];

    if(state->image.id != 0)
    {
        glDeleteTextures(1, &state->image.id);
        state->image.id = 0;
    }

    nav_free(&state->nav);
    nav_init(&state->nav);
    
    if (nav_scan_directory(&state->nav, dropped_file_path)) {
        state->image = rn_load_texture(nav_get_current_path(&state->nav));
    } else {
        state->image = rn_load_texture(dropped_file_path);
    }

    state->zoom = 1.0f;

    calculate_ratio_letterbox(window);
}