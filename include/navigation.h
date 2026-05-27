#ifndef NAVIGATION_H
#define NAVIGATION_H

#include<stdbool.h>
typedef struct navigation
{
    char **image_path;
    int image_count;
    int current_index;
    char current_directory[1024];
}file_navigation;

void nav_init(file_navigation *nav);
bool nav_scan_directory(file_navigation *nav, char *initial_image_path);
const char *nav_get_current_path(file_navigation *nav);
const char *nav_next_image(file_navigation *nav);
const char *nav_prev_image(file_navigation *nav);
void nav_free(file_navigation *nav);

#endif