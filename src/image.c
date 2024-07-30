#include "image.h"
#include "types.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

T_qword I_GetNextNumber(T_string str, T_qword startIndex, T_qword *endIndex) {
    T_qword index = startIndex;
    while(index < strlen(str) && !isdigit(str[index])) {
        index++;
    }

    if(index >= strlen(str)) {
        *endIndex = strlen(str);
        return 0;
    }

    char buffer[4] = {0};
    T_dword bufferOffset = 0;
    while(index < strlen(str) && isdigit(str[index]) && bufferOffset < 3) {
        buffer[bufferOffset] = str[index];
        index++;
        bufferOffset++;
    }

    *endIndex = index;
    return strtol(buffer, NULL, 10);
}

T_bool I_LoadImage(T_string path, T_image *image) {
    FILE *file = fopen(path, "r");

    T_qword read;
    T_qword lineLength = 0;
    T_qword lineIndex = 0;

    T_string line = NULL;
    T_qword dataOffset = 0;

    while((read = getline(&line, &lineLength, file)) != -1) {
        T_qword lineOffset = 0;
        // First line contains image width and height
        if(lineIndex == 0) {
            image->width = I_GetNextNumber(line, lineOffset, &lineOffset);
            image->height = I_GetNextNumber(line, lineIndex, &lineIndex); 
        
            image->data = (T_byte *)malloc(image->width * image->height);
            lineIndex++;
            continue;
        }

        while(lineOffset < strlen(line)) {
            T_byte colour = I_GetNextNumber(line, lineOffset, &lineOffset);
            if(lineOffset >= strlen(line)) continue;

            image->data[dataOffset] = colour;
            dataOffset++;
        }
    }

    return B_TRUE;
}