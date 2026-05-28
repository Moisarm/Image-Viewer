#define GLFW_INCLUDE_NONE
#include <stdio.h>
#include "glad/glad.h"
#include "callbacks/error-callback.h"
#include "color.h"
#include "navigation.h"

#include "callbacks/rezise-callback.h"
#include "calculate-ratio-letterbox.h"

#include "structs/global-state.h"

#define BACKGROUND_COLOR 0.3f, 0.1f, 0.4f, 1.0f

static global_state state;


void scroll_callback(GLFWwindow* window, double delta_x, double delta_y){
    float old_zoom = state.zoom;
    state.zoom += delta_y;
    state.zoom = MIN(state.zoom,15.0);
    state.zoom = MAX(state.zoom, 1.0f);

    float zoom_factor = state.zoom / old_zoom;
    double cursor_x_position, cursor_y_position;
    //Get the cursor position
    glfwGetCursorPos(window, &cursor_x_position, &cursor_y_position);

    state.image_position.x = (state.image_size.x * state.zoom > state.letterbox.x)? glm_clamp(cursor_x_position - (cursor_x_position - state.image_position.x) * zoom_factor, state.letterbox.x - (state.image_size.x * state.zoom), 0.0) :
    (state.letterbox.x - (state.image_size.x * state.zoom)) /2.0f;


    state.image_position.y = (state.image_size.y * state.zoom > state.letterbox.y)? glm_clamp(cursor_y_position - (cursor_y_position - state.image_position.y) * zoom_factor, state.letterbox.y - (state.image_size.y * state.zoom), 0.0) :
    (state.letterbox.y - (state.image_size.y * state.zoom)) /2.0f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        const char* next_path = NULL;

        if (key == GLFW_KEY_RIGHT) {
            next_path = nav_next_image(&state.nav);
        }
        else if (key == GLFW_KEY_LEFT) {
            next_path = nav_prev_image(&state.nav);
        }
        else if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        // if path change, should reload the texture
        if (next_path) {
            if (state.image.id != 0) {
                glDeleteTextures(1, &state.image.id);
            }
            state.image = rn_load_texture(next_path);
            state.zoom = 1.0f;
            calculate_ratio_letterbox(window);
        }
    }
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

    //Set context
    glfwMakeContextCurrent(window);

    //This creates a void pointer in the window variable that points to the "state" variable, so I can use it's values in other modules
    glfwSetWindowUserPointer(window, &state);

    //Set Callbacks
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Error initializing GLAD\n");
        return -1;
    }

    state.render = rn_init(SCREEN_WIDTH, SCREEN_HEIGHT, (RnGLLoader)glfwGetProcAddress);
    state.zoom = 1.0f;
    state.title_bar_size = 50;
    state.letterbox = (vec2s){
        (float)SCREEN_WIDTH,
        (float)SCREEN_HEIGHT - state.title_bar_size
    };

    nav_init(&state.nav);
    if (nav_scan_directory(&state.nav, "./gojo.jpg")) {
        // Load the image that returned the scan with it's current index
        state.image = rn_load_texture(nav_get_current_path(&state.nav));
    } else {
        // Temporary Fallback so if the scan fail, it load the texture
        state.image = rn_load_texture("./gojo.jpg");
    }
    
    calculate_ratio_letterbox(window);

    while (!glfwWindowShouldClose(window)) {
        //This function set the color of the screen but this doesn't paint the screen
        glClearColor(BACKGROUND_COLOR);
        //This is the function that paints the screen with the color set by the glClearColor 
        glClear(GL_COLOR_BUFFER_BIT);

        //rn_clear_color(rn_color_from_hex(0x191970ff));

        rn_begin(state.render);
        
        state.letterbox = (vec2s)
        {
            state.render->render_w, state.render->render_h - state.title_bar_size
        };
        
        state.cull_position = (vec2s)
        {
            (state.letterbox.x - state.image_size.x) /2.0f, (state.letterbox.y - state.image_size.y) /2.0f
        };
        
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

    nav_free(&state.nav);
    rn_terminate(state.render);  // Clean
    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Window Closed\n");
    return 0;
}