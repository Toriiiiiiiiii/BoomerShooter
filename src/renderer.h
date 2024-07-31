#pragma once
#include "world.h"
#include "actor.h"
#include "display.h"

void E_RenderScene(T_display *display, T_actor *player);
void E_RenderWall(T_display *display, T_actor *player, T_wall wall);