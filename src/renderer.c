#include "renderer.h"
#include "actor.h"
#include "display.h"
#include "log.h"
#include "math.h"
#include "types.h"
#include "world.h"
#include <raylib.h>

void E_RenderScene(T_display *display, T_actor *player) {
    // Create test sector to render
    T_sector testSector = {
        .nwalls = 4,
        .walls = {
            (T_wall){
                .x1 = -35, .y1 = 70,
                .x2 =  35, .y2 = 70,
                .facing = 2,
                .colour = 197
            },
            (T_wall){
                .x1 = -35, .y1 = 140,
                .x2 =  35, .y2 = 140,
                .facing = 0,
                .colour = 197
            },
            (T_wall){
                .x1 = -35, .y1 = 70,
                .x2 = -35, .y2 = 140,
                .facing = 3,
                .colour = 191
            },
            (T_wall){
                .x1 =  35, .y1 = 70,
                .x2 =  35, .y2 = 140,
                .facing = 1,
                .colour = 191
            },
        },
        .z = 0,
        .h = 40
    };

    E_RenderSector(display, player, testSector);
}

void E_RenderSector(T_display *display, T_actor *player, T_sector sector) {
    if(sector.nwalls > 64 || sector.nwalls < 3) {
        E_Log(E_LOG_ERR, "Attempting to draw sector with too many or too few walls.");
    }

    for(T_dword wallIndex = 0; wallIndex < sector.nwalls; ++wallIndex) {
        E_RenderWall(display, player, sector.walls[wallIndex], sector.z, sector.h);
    }
}

// Prevent division by 0 errors and clip wall if it is behind the player.
// Code is from video by 3dsage on Youtube: https://www.youtube.com/watch?v=huMO4VQEwPc&t=874s
void E_ClipBehindPlayer(T_sdword *x1, T_sdword *y1, T_sdword *z1, T_sdword x2, T_sdword y2, T_sdword z2) {
    T_float distA = *y1;
    T_float distB = y2;

    T_float dist = distA - distB;
    if(dist == 0 || dist == -0.0) dist = 1;         // Prevent division by 0

    T_float intersectionFactor = distA / dist;

    *x1 = *x1 + intersectionFactor * (x2 - *x1);
    *y1 = *y1 + intersectionFactor * (y2 - *y1); 
    *z1 = *z1 + intersectionFactor * (z2 - *z1);

    if(*y1 == 0) *y1 = 1;
}

// Draws a wall based on projected points.
// https://www.youtube.com/watch?v=huMO4VQEwPc&t=874s
void E_DrawWall(T_display *display, T_sdword x1, T_sdword x2, T_sdword y1, T_sdword y2, T_sdword t1, T_sdword t2, T_sdword depth1, T_sdword depth2, T_byte colour) {
    T_float dyTop = t2-t1;
    T_float dyBottom = y2-y1;
    T_float dx = x2 - x1;
    T_float dDepth = depth2 - depth1;
    T_float depthGrad = dDepth / dx;

    if(dx == 0) dx = 1;

    T_dword startX = x1;

    // Clip X coordinates
    if(x1 < 0) x1=0;
    if(x2 < 0) x2=0;
    if(x1 > display->width) x1 = display->width;
    if(x2 > display->width) x2 = display->width;

    for(T_dword x = x1; x < x2; ++x) {
        T_float xy1 = dyBottom * (x - startX + 0.5)/dx + y1;
        T_float xy2 = dyTop * (x - startX + 0.5)/dx + t1;
        T_float depth = depthGrad * (x - startX + 0.5) + depth1;

        // Clip y coordinates
        if(xy1 < 0) xy1 = 0;
        if(xy2 < 0) xy2 = 0;
        if(xy1 > display->height + 1) xy1 = display->height + 1;
        if(xy2 > display->height + 1) xy2 = display->height + 1;

        for(T_dword y = xy1; y < xy2; ++y) {
            if(display->zbuffer[y * display->width + x] < depth) continue;

            display->zbuffer[y * display->width + x] = depth;
            E_SetPixel(display, x, (display->height) - y, colour);
        }
    }
}

// Project the wall points and draw the wall.
// Adapted from https://www.youtube.com/watch?v=huMO4VQEwPc&t=874s to fit the needs of the engine.
void E_RenderWall(T_display *display, T_actor *player, T_wall wall, T_sdword z, T_sdword h) {
    T_float swap;
    switch(wall.facing) {
        case 0:
            if(wall.x2 < wall.x1) {
                swap = wall.x1;
                wall.x1 = wall.x2;
                wall.x2 = swap;

                swap = wall.y1;
                wall.y1 = wall.y2;
                wall.y2 = swap;
            }
            break;

        case 1:
            if(wall.y1 < wall.y2) {
                swap = wall.x1;
                wall.x1 = wall.x2;
                wall.x2 = swap;

                swap = wall.y1;
                wall.y1 = wall.y2;
                wall.y2 = swap;
            }
            break;

        case 2:
            if(wall.x1 < wall.x2) {
                swap = wall.x1;
                wall.x1 = wall.x2;
                wall.x2 = swap;

                swap = wall.y1;
                wall.y1 = wall.y2;
                wall.y2 = swap;
            }
            break;

        case 3:
            if(wall.y2 < wall.y1) {
                swap = wall.x1;
                wall.x1 = wall.x2;
                wall.x2 = swap;

                swap = wall.y1;
                wall.y1 = wall.y2;
                wall.y2 = swap;
            }
            break;
    }

    T_dword plrAngle = player->angle;
    T_float plrAngleRad = (T_float)(plrAngle) * M_PI / 180.0;

    T_float sinAngle = sin(plrAngleRad);
    T_float cosAngle = cos(plrAngleRad);

    // Translate wall x-coordinates
    T_float wx1 = (T_float)(wall.x1 - player->x);
    T_float wx2 = (T_float)(wall.x2 - player->x);

    // Translate wall y-coordinates
    T_float wy1 = (T_float)(wall.y1 - player->y);
    T_float wy2 = (T_float)(wall.y2 - player->y);

    T_sdword wz = (T_float)(z - player->z);
    T_sdword wz2 = (T_float)(wz + h);

    // Rotate wall point 1
    T_sdword rotx1 = (cosAngle*wx1) - (sinAngle*wy1);
    T_sdword roty1 = (sinAngle*wx1) + (cosAngle*wy1);

    // Rotate wall point 2
    T_sdword rotx2 = (cosAngle*wx2) - (sinAngle*wy2);
    T_sdword roty2 = (sinAngle*wx2) + (cosAngle*wy2);

    T_sdword rotx3 = rotx1;
    T_sdword roty3 = roty1;
    T_sdword rotx4 = rotx2;
    T_sdword roty4 = roty2;

    // If the wall is behind the player, don't bother drawing it.
    if(roty1 < 1 && roty2 < 1) return;

    // Clip wall points if they are behind the player.
    if(roty1 < 1) {
        E_ClipBehindPlayer(&rotx1, &roty1, &wz, rotx2, roty2, wz);
        E_ClipBehindPlayer(&rotx3, &roty3, &wz2, rotx4, roty4, wz2);
    }

    if(roty2 < 1) {
        E_ClipBehindPlayer(&rotx2, &roty2, &wz, rotx1, roty1, wz);
        E_ClipBehindPlayer(&rotx4, &roty4, &wz2, rotx3, roty3, wz2);
    }

    // Perform Perspective Division
    T_float px1 = (T_float)(rotx1*200)/roty1 + (T_float)display->width / 2;
    T_float py1 = (T_float)(wz*200)/roty1 + (T_float)display->height/ 2;

    T_float px2 = (T_float)(rotx2*200)/roty2 + (T_float)display->width / 2;
    T_float py2 = (T_float)(wz*200)/roty2 + (T_float)display->height/ 2;

    T_float topx1 = (T_float)(rotx3*200)/roty3 + (T_float)display->width / 2;
    T_float topy1 = (T_float)(wz2*200)/roty1 + (T_float)display->height/ 2;
    T_float topx2 = (T_float)(rotx4*200)/roty4 + (T_float)display->width / 2;
    T_float topy2 = (T_float)(wz2*200)/roty4 + (T_float)display->height/ 2;

    E_DrawWall(display, px1, px2, py1, py2, topy1, topy2, roty1, roty2, wall.colour);
}