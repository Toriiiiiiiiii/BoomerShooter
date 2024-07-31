#pragma once
#include "types.h"

struct _T_wall {
    T_sdword x1, y1;
    T_sdword x2, y2;
    T_byte facing; // 0 = -y, 1 = -x, 2 = y, 3 = x
    T_byte colour;
};

typedef struct _T_wall T_wall;

struct _T_sector {
    T_byte nwalls;     // Max 64
    T_wall walls[64];

    T_sdword z, h;
};

typedef struct _T_sector T_sector;