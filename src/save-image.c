#include "structs/global-state.h"
#include "stb/stb-image-write.h"

void save_image(GLFWwindow* window, global_state* state){
    // get image size
    int width = (int)(state->image_size.x * state->zoom);
    int height = (int)(state->image_size.y * state->zoom);

    if (width <= 0 || height <= 0) return;

    // 2. save memory for rgb channels (4 bytes per píxel)
    unsigned char* pixels = (unsigned char*)malloc(4 * width * height);
    if (!pixels) {
        fprintf(stderr, "Error: No se pudo asignar memoria para guardar la imagen.\n");
        return;
    }

    // 3. edit opengl pixel align 
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // 4. Read the pixels from the screen buffer (Framebuffers)
    glReadPixels(
        (int)state->cull_position.x, 
        (int)state->cull_position.y, 
        width, 
        height, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        pixels
    );

    unsigned char* flipped_pixels = (unsigned char*)malloc(4 * width * height);
    for (int y = 0; y < height; y++) {
        memcpy(&flipped_pixels[y * width * 4], &pixels[(height - 1 - y) * width * 4], width * 4);
    }

    // 6. Generate the new name
    char new_path[512];
    snprintf(new_path, sizeof(new_path), "./copy_%s", ""); 

    // 7. Write the new file using stb
    if (stbi_write_jpg(new_path, width, height, 4, flipped_pixels, 90)) {
        printf("¡Imagen guardada con éxito en: %s!\n", new_path);
    } else {
        fprintf(stderr, "Error al escribir el archivo en el disco.\n");
    }

    // Freeing memory
    free(pixels);
    free(flipped_pixels);
}