#include<stdio.h>

int get_opengl_color(int red, int green, int blue){
    float r = red / 255.0f;
    float g = green / 255.0f;
    float b = blue / 255.0f;
    
    printf("%f, %f, %f, /n",r,g,b);
    return 0;
}