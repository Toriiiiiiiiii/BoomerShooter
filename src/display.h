#pragma once
#include "image.h"
#include "types.h"

struct _T_display {
    T_dword width;
    T_dword height;
    T_dword pixelScale;

    T_byte *display;
    T_qword *zbuffer;

    T_qword *palette;
};

typedef struct _T_display T_display;

T_display E_InitDisplay(T_dword width, T_dword height, T_dword pixelScale, T_string palettePath);
void      E_DestroyDisplay(T_display *display);
T_bool    E_LoadPalette(T_string path, T_qword *palette);

void E_ClearDisplay(T_display *display);
void E_RenderDisplay(T_display *display);
void E_SetPixel(T_display *display, T_dword x, T_dword y, T_byte c);
void E_DrawImage(T_display *display, T_dword x, T_dword y, T_image img);