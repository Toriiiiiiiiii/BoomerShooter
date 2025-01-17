#include "display.h"
#include "types.h"
#include "log.h"

#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

T_display E_InitDisplay(T_dword width, T_dword height, T_dword pixelScale, T_string palettePath) {
    SetTraceLogLevel(LOG_ERROR);

    T_display result = {0};

    // Initialize basic variables.
    result.width = width;
    result.height = height;
    result.pixelScale = pixelScale;

    // Allocate memory for display buffer, z-buffer and the palette array.
    result.display = (T_byte *)malloc(width * height * sizeof(T_byte));
    result.zbuffer = (T_qword *)malloc(width * height * sizeof(T_qword));

    for(T_dword index = 0; index < width * height; ++index) {
        result.zbuffer[index] = 999999;
    }

    result.palette = (T_qword *)malloc(256 * sizeof(T_qword));

    E_Log(E_LOG_INFO, "Initialized Display Buffer");

    // Load the palette, and return an empty display structure if loading failed.
    if(!E_LoadPalette(palettePath, result.palette)) return (T_display){0};

    // Cap FPS to 30 for consistency.
    SetTargetFPS(30);

    // Create window using Raylib.
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
    // Palettes are stored as 16x16 PNG images.
    Image image = LoadImage(path);

    // Halt if palette was not found.
    if(!IsImageReady(image)) {
        E_Log(E_LOG_ERR, "Could not load palette file");
        return false;
    }

    // Halt if the image is not the correct size.
    if(image.height * image.width != 256) {
        E_Log(E_LOG_ERR, "Invalid palette size!");
        return false;
    }

    // Write image data to the palette.
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
    // Clear the display background.
    for(int index = 0; index < display->width * display->height; ++index) {
        display->display[index] = E_BACKGROUND_COLOUR;
    }
}

void E_RenderDisplay(T_display *display) {
    BeginDrawing();
    ClearBackground(BLACK);

    // Render the display buffer.
    int x = 0, y = 0;
    for(int index = 0; index < display->width * display->height; ++index) {
        T_qword colour = display->palette[ display->display[index] ];

        // DrawRectangle is used instead of DrawPixel to accomodate for pixel scaling.
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

    char fpsString[256] = {0};
    snprintf(fpsString, 256, "%d", GetFPS());
    DrawText( fpsString, 0, 0, 20, RAYWHITE);

    EndDrawing();

    // Clear zbuffer
    for(T_dword index = 0; index < display->width * display->height; ++index) {
        display->zbuffer[index] = 999999;
    }
}

void E_SetPixel(T_display *display, T_dword x, T_dword y, T_byte c) {
    // Colour Index 0 is used for transparency.
    if(c == 0) return;

    // Prevent buffer overflow
    if(x >= display->width || y >= display->height) return;

    display->display[y * display->width + x] = c;
}

void E_DrawImage(T_display *display, T_dword x, T_dword y, T_image img) {
    // Iterate through each pixel in the image and draw it to the display buffer.
    for(T_dword imgY = 0; imgY < img.height; ++imgY) {
        for(T_dword imgX = 0; imgX < img.width; ++imgX) {
            E_SetPixel(display, x+imgX, y+imgY, img.data[imgY * img.width + imgX]);
        }
    }
}