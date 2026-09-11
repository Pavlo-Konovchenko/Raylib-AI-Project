/* =====================================================================
   RAYLIB 2D TUTORIAL - STEPS 1-6
   =====================================================================
   The code is split across a few files by responsibility, so this file
   can stay a short, readable summary of "what happens, in order":

     game.h / game.c     -> Step 1: window setup & the game loop shell
     shapes.h / shapes.c -> Step 2: reference shapes & the coordinate system
     player.h / player.c -> Steps 3-6: input, movement, sprite, animation,
                             and facing direction

   Read this file first to see the overall shape of the program, then
   open whichever file covers the step you want the details on - each
   has its own teaching comments at the top.
   ===================================================================== */

#include "raylib.h"
#include "game.h"
#include "shapes.h"
#include "player.h"

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 540;

    /* --- STEP 1: setup -------------------------------------------- */
    InitGame(screenWidth, screenHeight, "Steps 1-6 - Window, Drawing, Input & Directional Sprite");

    /* --- STEPS 3/4: setup -----------------------------------------------
       InitPlayer loads the sprite texture and figures out how big a
       single frame is - see player.c for why. */
    Vector2 playerPos;
    float playerSpeed;
    Texture2D playerTexture;
    int frameWidth;
    int frameHeight;
    InitPlayer(&playerPos, &playerSpeed, &playerTexture,
               &frameWidth, &frameHeight, screenWidth, screenHeight);

    /* --- STEP 5: setup -----------------------------------------------
       Animation state: which of the 4 frames we're currently showing,
       and how much time has passed since we last advanced to the next
       one. Both start at zero/idle. */
    int currentFrame = 0;
    float frameTimer = 0.0f;
    bool isMoving = false;

    /* --- STEP 6: setup -------------------------------------------------
       Which row of the sheet (i.e. which direction) we're facing. Start
       facing down - a common default for a top-down character. */
    int facingRow = ROW_DOWN;

    /* --- STEP 1: the game loop ------------------------------------- */
    while (!WindowShouldClose())
    {
        /* UPDATE */
        UpdatePlayer(&playerPos, playerSpeed, frameWidth, frameHeight, screenWidth, screenHeight,
                     &currentFrame, &frameTimer, &isMoving, &facingRow);

        /* DRAW */
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawReferenceShapes();                                                          /* STEP 2 */
            DrawPlayer(playerPos, playerTexture, frameWidth, frameHeight, currentFrame, facingRow); /* STEPS 4/5/6 */

        EndDrawing();
    }

    /* --- STEP 1/4: shutdown ----------------------------------------- */
    ShutdownGame(playerTexture);

    return 0;
}
