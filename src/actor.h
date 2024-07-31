#pragma once
#include "types.h"

enum _T_actortype {
    ACTOR_NULL,
    ACTOR_PLAYER,
    ACTOR_ITEM,
};

typedef enum _T_actortype T_actortype;

struct _T_actor {
    T_actortype type;

    T_sdword x;
    T_sdword y;
    T_sdword z;
    T_float angle;
};

typedef struct _T_actor T_actor;