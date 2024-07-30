#include <raylib.h>
#include "display.h"

int main() {
    T_display display = E_InitDisplay(320, 200, 3, "assets/palette.png");

    while(!WindowShouldClose()) {
        E_ClearDisplay(&display);
        E_RenderDisplay(&display);
    }

    CloseWindow();
    return 0;
}