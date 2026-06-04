#ifndef GLOBAL_STATE_STRUCT_H
#define GLOBAL_STATE_STRUCT_H

#include "file-navigation.h"
#include <GLFW/glfw3.h>
#include <runara/include/runara/runara.h>

//macros
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800


typedef struct {
    RnState* render;
    
    float zoom;
    
    vec2s image_position, image_size, cull_position;

    vec2s letterbox;
    
    RnTexture image;

    float title_bar_size;

    file_navigation nav;

    bool is_full_screen;

    int window_width, window_height; 

    int window_x, window_y;

    RnFont* font;

    float rotation_angle;

    bool horizontal_flip, vertical_flip;
} global_state; 

#endif