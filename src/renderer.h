#pragma once
#include "types.h"
#include "world.h"
#include "actor.h"
#include "display.h"

void E_RenderScene(T_display *display, T_actor *player);
void E_RenderSector(T_display *display, T_actor *player, T_sector sector);
void E_RenderWall(T_display *display, T_actor *player, T_wall wall, T_sdword z, T_sdword h);