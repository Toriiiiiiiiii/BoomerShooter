#include "util.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS_IN_NUMBER 10
#define NUM_BUFF_SIZE 11

T_qword U_GetNextNumber(T_string str, T_qword startIndex, T_qword *endIndex) {
    T_qword index = startIndex;

    // Skip any non-digit characters.
    while(index < strlen(str) && !isdigit(str[index])) {
        index++;
    }

    // Stop if end of string was reached.
    if(index >= strlen(str)) {
        *endIndex = strlen(str);
        return 0;
    }

    // Buffer for storing the number string.
    // BUFF_SIZE should be one more than MAX_CHARS_IN_NUM to allow for null-terminator.
    char buffer[NUM_BUFF_SIZE] = {0};
    T_dword bufferOffset = 0;

    // Copy the number string into the buffer
    while(index < strlen(str) && isdigit(str[index]) && bufferOffset < MAX_CHARS_IN_NUMBER) {
        buffer[bufferOffset] = str[index];
        index++;
        bufferOffset++;
    }

    // Output the pointer to the string after the number and return the integer.
    *endIndex = index;
    return strtol(buffer, NULL, 10);
}