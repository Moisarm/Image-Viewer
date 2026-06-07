#include "structs/global-state.h"
#include "navigation.h"
#include "calculate-ratio-letterbox.h"
#include "save-image.h"

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
        else if (key == GLFW_KEY_F11){
            if(!state->is_full_screen){
                //set as true the is_full_screen property
                state->is_full_screen = !state->is_full_screen;

                //Save current position and size of the screen
                glfwGetWindowPos(window, &state->window_x, &state->window_y);
                glfwGetWindowSize(window, &state->window_width, &state->window_height);


                //Get the principal monitor and and its video mode
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                //Activate the full screen
                glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

                
            }else{

                state->is_full_screen = !state->is_full_screen;
                
                if (state->window_width <= 0 || state->window_height <= 0) {
                    state->window_width = SCREEN_WIDTH;
                    state->window_height = SCREEN_HEIGHT;
                }

                //Deactivate the full screen and set the window to it's previous position and size
                glfwSetWindowMonitor(window, NULL, state->window_x, state->window_y, state->window_width, state->window_height,0);
                
               
                calculate_ratio_letterbox(window);
            }
        }
       
        //Rotate
        else if (key == GLFW_KEY_R) {
            state->rotation_angle += 90.0f;
            if (state->rotation_angle >= 360.0f) {
                state->rotation_angle = 0.0f;
            }
        }
        
        //Vertical Flip
        else if (key == GLFW_KEY_H) {
            state->horizontal_flip = !state->horizontal_flip;
        }
        
        //Horizontal Flip
        else if (key == GLFW_KEY_V) {
            state->vertical_flip = !state->vertical_flip;
        }

        //save image
        if (key == GLFW_KEY_S && (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
        {
            
            save_image(window, state);
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