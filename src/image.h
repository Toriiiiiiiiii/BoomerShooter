#pragma once
#include <stdio.h>

#include "types.h"

struct _T_image {
    T_dword width;
    T_dword height;

    T_byte *data;
};

typedef struct _T_image T_image;

// Loads an image structure from a text file.
T_bool I_LoadImage(T_string path, T_image *image);

// Deallocates an image structure.
void I_DestroyImage(T_image *img);