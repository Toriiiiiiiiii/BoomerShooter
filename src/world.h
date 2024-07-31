#pragma once
#include "types.h"

struct _T_wall {
    T_sdword x1, y1;
    T_sdword x2, y2;

    T_sdword z;
    T_sdword h;
};

typedef struct _T_wall T_wall;