#include <math.h>
#include <raylib.h>
#include <stdio.h>

#include "actor.h"
#include "display.h"
#include "image.h"
#include "log.h"
#include "renderer.h"

int main() {
    T_display display = E_InitDisplay(320, 200, 3, "assets/palette.png");

    T_image img = {0};
    I_LoadImage("assets/smiley.txt", &img);
    
    char message[256] = {0};
    snprintf(message, 256, "Image Dimensions: %dx%d", img.width, img.height);

    E_Log(E_LOG_INFO, message);

    T_actor player = (T_actor){
        .type = ACTOR_PLAYER,
        .x = 70,
        .y = -110,
        .z = 20,
        .angle = 0.0,
    };

    T_dword frameCounter = 0;
    while(!WindowShouldClose()) {
        if(IsKeyDown(KEY_LEFT)) {
            player.angle -= 1;
        }
        if(IsKeyDown(KEY_RIGHT)) {
            player.angle += 1;
        }

        if(player.angle < 0) player.angle += 359;
        if(player.angle > 360) player.angle -= 359; 

        T_float dx = sin(player.angle * M_PI/180) * 10;
        T_float dy = cos(player.angle * M_PI/180) * 10;
        if(IsKeyDown(KEY_W)) {
            player.x += dx;
            player.y += dy;
        }

        if(IsKeyDown(KEY_S)) {
            player.x -= dx;
            player.y -= dy;
        }

        if(IsKeyDown(KEY_D)) {
            player.x += dy;
            player.y -= dx;
        }

        if(IsKeyDown(KEY_A)) {
            player.x -= dy;
            player.y += dx;
        }

        E_ClearDisplay(&display);

        //E_DrawImage(&display, 0, 0, img);
        E_RenderScene(&display, &player);

        E_RenderDisplay(&display);
        frameCounter ++;
    }

    I_DestroyImage(&img);
    E_DestroyDisplay(&display);
    return 0;
}