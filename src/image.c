#include "image.h"
#include "types.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

T_bool I_LoadImage(T_string path, T_image *image) {
    FILE *file = fopen(path, "r");

    if(!file) return B_FALSE;

    T_qword read;
    T_qword lineLength = 0;
    T_qword lineIndex = 0;

    T_string line = NULL;
    T_qword dataOffset = 0;

    // Reads each line into a variable. Stops the loop if EOF is reached.
    while((read = getline(&line, &lineLength, file)) != -1) {
        T_qword lineOffset = 0;
        // First line contains image width and height
        if(lineIndex == 0) {
            image->width = U_GetNextNumber(line, lineOffset, &lineOffset);
            image->height = U_GetNextNumber(line, lineIndex, &lineIndex); 
        
            image->data = (T_byte *)malloc(image->width * image->height);
            lineIndex++;
            continue;
        }

        while(lineOffset < strlen(line)) {
            T_byte colour = U_GetNextNumber(line, lineOffset, &lineOffset);
            
            // Prevent writing image data if end of line is reached.
            // This is a hacky solution to an issue created by U_GetNextNumber().
            if(lineOffset >= strlen(line)) continue;

            image->data[dataOffset] = colour;
            dataOffset++;
        }
    }

    return B_TRUE;
}

void I_DestroyImage(T_image *img) {
    img->width = img->height = 0;
    free(img->data);
}