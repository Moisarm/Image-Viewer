#ifndef FILE_NAVIGATION_STRUCT_H
#define FILE_NAVIGATION_STRUCT_H

typedef struct 
{
    char **image_path;
    int image_count;
    int current_index;
    char current_directory[1024];
}file_navigation;

#endif