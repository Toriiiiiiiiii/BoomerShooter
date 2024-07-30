#include "display.h"
#include "types.h"
#include "log.h"

#include <raylib.h>
#include <stdlib.h>

T_display E_InitDisplay(T_dword width, T_dword height, T_dword pixelScale, T_string palettePath) {
    SetTraceLogLevel(LOG_ERROR);

    T_display result = {0};

    result.width = width;
    result.height = height;
    result.pixelScale = pixelScale;
    result.display = (T_byte *)malloc(width * height * sizeof(T_byte));
    result.zbuffer = (T_qword *)malloc(width * height * sizeof(T_qword));
    result.palette = (T_qword *)malloc(256 * sizeof(T_qword));

    E_Log(E_LOG_INFO, "Initialized Display Buffer");

    if(!E_LoadPalette(palettePath, result.palette)) return (T_display){0};

    SetTargetFPS(30);

    InitWindow(width * pixelScale, height * pixelScale, "BOOMER SHOOTER PROTOTYPE");
    E_Log(E_LOG_INFO, "Initialized Game Window");
    return result;
}

void E_DestroyDisplay(T_display *display) {
    CloseWindow();

    display->width = display->height = 0;
    free(display->display);
    free(display->zbuffer);
    free(display->palette);
}

T_bool E_LoadPalette(T_string path, T_qword *palette) {
    Image image = LoadImage(path);
    if(!IsImageReady(image)) {
        E_Log(E_LOG_ERR, "Could not load palette file");
        return false;
    }

    if(image.height * image.width != 256) {
        E_Log(E_LOG_ERR, "Invalid palette size!");
        return false;
    }

    T_byte *data = (T_byte *)image.data;
    for(T_qword index = 0; index < image.width * image.height * 4; index += 4) {
        T_qword colour = 0;

        colour |= data[index + 0] << 24;
        colour |= data[index + 1] << 16;
        colour |= data[index + 2] << 8;
        colour |= data[index + 3] << 0;

        palette[index / 4] = colour;
    }

    E_Log(E_LOG_INFO, "Loaded Colour Palette");
    return true;
}

void E_ClearDisplay(T_display *display) {
    for(int index = 0; index < display->width * display->height; ++index) {
        display->display[index] = 207;
    }
}

void E_RenderDisplay(T_display *display) {
    BeginDrawing();
    ClearBackground(BLACK);

    int x = 0, y = 0;
    for(int index = 0; index < display->width * display->height; ++index) {
        T_qword colour = display->palette[ display->display[index] ];

        DrawRectangle(
            x * display->pixelScale, 
            y * display->pixelScale, 
            display->pixelScale, 
            display->pixelScale, 
            (Color){.r=(colour >> 24) & 0xff, .g=(colour >> 16) & 0xff, .b=(colour >> 8) & 0xff, .a=0xff}
        );

        x++;

        if(x >= display->width) {
            y++;
            x = 0;
        }
    }

    EndDrawing();
}

void E_SetPixel(T_display *display, T_dword x, T_dword y, T_byte c) {
    // Colour Index 0 is used for transparency.
    if(c == 0) return;

    // Prevent buffer overflow
    if(x >= display->width || y >= display->height) return;

    display->display[y * display->width + x] = c;
}

void E_DrawImage(T_display *display, T_dword x, T_dword y, T_image img) {
    for(T_dword imgY = 0; imgY < img.height; ++imgY) {
        for(T_dword imgX = 0; imgX < img.width; ++imgX) {
            E_SetPixel(display, x+imgX, y+imgY, img.data[imgY * img.width + imgX]);
        }
    }
}