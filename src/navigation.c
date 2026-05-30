#include "navigation.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>


//Static turns this function into private
static bool is_image(const char *filename){
    const char *ext = strrchr(filename, '.');
    if (!ext) return false;
    return strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".png") == 0 || strcmp(ext, ".bmp") == 0 || strcmp(ext, ".gif") == 0;
}

void nav_init(file_navigation *nav){
    nav->image_path = NULL;
    nav->image_count = 0;
    nav->current_index = -1;
    memset(nav->current_directory, 0, sizeof(nav->current_directory));
}

bool nav_scan_directory(file_navigation *nav, const char *initial_image_path) {
    char path_copy[1024];
    strncpy(path_copy, initial_image_path, sizeof(path_copy));
    
    char* last_slash = strrchr(path_copy, '/');
    if (last_slash) {
        *last_slash = '\0';
        strncpy(nav->current_directory, path_copy, sizeof(nav->current_directory));
    } else {
        strcpy(nav->current_directory, ".");
    }

    DIR* dir = opendir(nav->current_directory);
    if (!dir) return false;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (is_image(entry->d_name)) {
            nav->image_path = realloc(nav->image_path, sizeof(char*) * (nav->image_count + 1));
            
            char full_path[2048];
            snprintf(full_path, sizeof(full_path), "%s/%s", nav->current_directory, entry->d_name);
            nav->image_path[nav->image_count] = strdup(full_path);

            if (strcmp(full_path, initial_image_path) == 0 || 
                (strcmp(nav->current_directory, ".") == 0 && strcmp(entry->d_name, initial_image_path) == 0)) {
                nav->current_index = nav->image_count;
            }
            nav->image_count++;
        }
    }
    closedir(dir);

    if (nav->current_index == -1 && nav->image_count > 0) {
        nav->current_index = 0;
    }

    return nav->image_count > 0;
}

const char *nav_get_current_path(file_navigation* nav) {
    if (nav->current_index >= 0 && nav->current_index < nav->image_count) {
        return nav->image_path[nav->current_index];
    }
    return NULL;
}

const char *nav_next_image(file_navigation* nav) {
    if (nav->image_count == 0) return NULL;
    nav->current_index = (nav->current_index + 1) % nav->image_count;
    return nav_get_current_path(nav);
}

const char *nav_prev_image(file_navigation* nav) {
    if (nav->image_count == 0) return NULL;
    nav->current_index = (nav->current_index - 1 + nav->image_count) % nav->image_count;
    return nav_get_current_path(nav);
}

void nav_free(file_navigation* nav) {
    if (nav->image_path) {
        for (int i = 0; i < nav->image_count; i++) {
            free(nav->image_path[i]);
        }
        free(nav->image_path);
        nav->image_path = NULL;
    }
    nav->image_count = 0;
    nav->current_index = -1;
}