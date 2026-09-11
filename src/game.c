#include "game.h"

void InitGame(int screenWidth, int screenHeight, const char *title)
{
    /* InitWindow(width, height, title) creates the OS window and gets
       raylib's graphics system ready. Nothing else in raylib works until
       this has been called - that includes loading textures (Step 4),
       which is why sprite loading has to happen AFTER this. */
    InitWindow(screenWidth, screenHeight, title);

    /* SetTargetFPS caps how many times per second the loop in main() runs.
       60 is a common target for simple 2D games. Without this, the loop
       would run as fast as the CPU allows, which wastes power and (as
       Step 3 explains) makes movement speed depend on how fast the
       computer is. */
    SetTargetFPS(60);
}

void ShutdownGame(Texture2D playerTexture)
{
    /* Anything loaded onto the GPU (textures, sounds, etc.) must be
       unloaded BEFORE CloseWindow() destroys the graphics context.
       Forgetting this leaks GPU memory - harmless for a quick test, but
       a real bug in a long-running game. */
    UnloadTexture(playerTexture);

    CloseWindow();
}
