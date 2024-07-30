#include <raylib.h>
#include "display.h"
#include "image.h"
#include "log.h"
#include <stdio.h>

int main() {
    T_display display = E_InitDisplay(320, 200, 3, "assets/palette.png");

    T_image img = {0};
    I_LoadImage("assets/smiley.txt", &img);
    
    char message[256] = {0};
    snprintf(message, 256, "Image Dimensions: %dx%d", img.width, img.height);

    E_Log(E_LOG_INFO, message);

    while(!WindowShouldClose()) {
        E_ClearDisplay(&display);

        E_DrawImage(&display, 0, 0, img);

        E_RenderDisplay(&display);
    }

    I_DestroyImage(&img);
    E_DestroyDisplay(&display);
    return 0;
}