#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "structs/file-navigation.h"
#include<stdbool.h>


void nav_init(file_navigation *nav);
bool nav_scan_directory(file_navigation *nav, char *initial_image_path);
const char *nav_get_current_path(file_navigation *nav);
const char *nav_next_image(file_navigation *nav);
const char *nav_prev_image(file_navigation *nav);
void nav_free(file_navigation *nav);

#endif