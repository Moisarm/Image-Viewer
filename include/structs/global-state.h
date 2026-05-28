#ifndef GLOBAL_STATE_STRUCT_H
#define GLOBAL_STATE_STRUCT_H

#include "file-navigation.h"
#include <GLFW/glfw3.h>
#include <runara/include/runara/runara.h>

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b


typedef struct {
    RnState* render;
    
    float zoom;
    
    vec2s image_position, image_size, cull_position;

    vec2s letterbox;
    
    RnTexture image;

    float title_bar_size;

    file_navigation nav;
} global_state;

#endif