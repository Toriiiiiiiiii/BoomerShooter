#pragma once
#include "image.h"
#include "types.h"

#ifndef E_BACKGROUND_COLOUR
    #define E_BACKGROUND_COLOUR 207
#endif // E_BACKGROUND_COLOUR

struct _T_display {
    T_dword width;
    T_dword height;
    T_dword pixelScale;

    T_byte *display;
    T_qword *zbuffer;

    T_qword *palette;
};

typedef struct _T_display T_display;

// Initializes a T_display structure and creates a window for rendering.
T_display E_InitDisplay(T_dword width, T_dword height, T_dword pixelScale, T_string palettePath);

// Closes Raylib and destroys an allocated T_display structure.
void E_DestroyDisplay(T_display *display);

// Loads a palette from a 16x16 image file.
T_bool E_LoadPalette(T_string path, T_qword *palette);

// Clears the display. The colour of the background is set by E_BACKGROUND_COLOUR
void E_ClearDisplay(T_display *display);

// Renders the display buffer to the window.
void E_RenderDisplay(T_display *display);

// Sets a pixel at coordinates (x,y) to colour c.
void E_SetPixel(T_display *display, T_dword x, T_dword y, T_byte c);

// Draws an image at coordinates (x,y).
void E_DrawImage(T_display *display, T_dword x, T_dword y, T_image img);