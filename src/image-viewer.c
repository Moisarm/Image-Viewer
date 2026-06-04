#define GLFW_INCLUDE_NONE
#include <stdio.h>
#include "glad/glad.h"
#include "navigation.h"

#include "callbacks/error-callback.h"
#include "callbacks/rezise-callback.h"
#include "callbacks/scroll-callback.h"
#include "callbacks/key-callback.h"
#include "callbacks/drag-and-drop-callback.h"

#include "calculate-ratio-letterbox.h"

#include "structs/global-state.h"

#define BACKGROUND_COLOR 0.3f, 0.1f, 0.4f, 1.0f

static global_state state;

int main() {
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
    glfwSetDropCallback(window, drag_and_drop_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Error initializing GLAD\n");
        return -1;
    }

    state.render = rn_init(SCREEN_WIDTH, SCREEN_HEIGHT, (RnGLLoader)glfwGetProcAddress);
    state.zoom = 1.0f;
    state.rotation_angle = 0.0f;
    state.vertical_flip= false;
    state.horizontal_flip = false;
    state.title_bar_size = 40;
    state.is_full_screen = false;
    state.letterbox = (vec2s){
        (float)SCREEN_WIDTH,
        (float)SCREEN_HEIGHT - state.title_bar_size
    };

    state.font = rn_load_font(state.render, "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16);
    if (!state.font) {
        // Fallback that use free sans if Dejavu is not found
        state.font = rn_load_font(state.render, "/usr/share/fonts/truetype/freefont/FreeSans.ttf", 16);
    }
    
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
            (float)state.render->render_w,
            (float)state.render->render_h - state.title_bar_size
        };
        
        state.cull_position = (vec2s)
        {
            (state.letterbox.x - state.image_size.x) /2.0f, ((state.letterbox.y - state.image_size.y) /2.0f) + state.title_bar_size
        };

        char metadata_buffer[256];
        const char* current_path = nav_get_current_path(&state.nav);
        const char* file_name = "Sin archivo";

        if (current_path != NULL) {
            const char* last_slash = strrchr(current_path, '/');
            #ifdef _WIN32
            if (!last_slash) last_slash = strrchr(current_path, '\\');
            #endif

            if (last_slash) {
                file_name = last_slash + 1;
            } else {
                file_name = current_path;
            }
        }

        int total_images = state.nav.image_count;
        int current_idx = state.nav.current_index;

        snprintf(metadata_buffer, sizeof(metadata_buffer),
            " %s  |  %dx%d px  |  Zoom: %d%%  |  [%d / %d]",
            file_name,
            state.image.width, state.image.height,
            (int)(state.zoom * 100.0f),
            total_images > 0 ? (current_idx + 1) : 0,
            total_images);

        // title bar size position
        vec2s text_position = (vec2s){ 15.0f, 15.0f };

        rn_text_render(
            state.render, 
            metadata_buffer,            
            state.font, 
            text_position,              
            RN_WHITE                    
        );
        
        rn_set_cull_start_x(state.render, state.cull_position.x);
        rn_set_cull_end_x(state.render, state.cull_position.x + state.image_size.x);

        rn_set_cull_start_y(state.render, state.cull_position.y);
        rn_set_cull_end_y(state.render, state.cull_position.y + state.image_size.y);

        vec2s custom_texcoords[4] = {
            (vec2s){0.0f, 0.0f}, // Bottom-Left
            (vec2s){1.0f, 0.0f}, // Bottom-Right
            (vec2s){1.0f, 1.0f}, // Top-Right
            (vec2s){0.0f, 1.0f}  // Top-Left
        };

        if (state.horizontal_flip) {
            custom_texcoords[0].x = 1.0f;
            custom_texcoords[1].x = 0.0f;
            custom_texcoords[2].x = 0.0f;
            custom_texcoords[3].x = 1.0f;
        }
        if (state.vertical_flip) {
            custom_texcoords[0].y = 1.0f;
            custom_texcoords[1].y = 1.0f;
            custom_texcoords[2].y = 0.0f;
            custom_texcoords[3].y = 0.0f;
        }

        rn_image_render_adv(
            state.render, 
            state.image_position, 
            state.rotation_angle, 
            RN_WHITE, 
            (RnTexture){
                .width = (uint32_t)(state.image_size.x * state.zoom),
                .height = (uint32_t)(state.image_size.y * state.zoom),
                .id = state.image.id
            },
            custom_texcoords,     
            false,                
            RN_NO_COLOR,          
            0.0f, 
            0.0f
        );
        
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