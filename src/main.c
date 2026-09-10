/* =====================================================================
   RAYLIB 2D TUTORIAL - STEPS 1-3
   =====================================================================
   This file walks through three ideas. Instead of cramming everything
   into main(), each step gets its own function(s), so main() stays a
   short, readable summary of "what happens, in order" - and the
   teaching comments live inside the function that they explain.

     STEP 1: The window and the game loop      -> InitGame / ShutdownGame
     STEP 2: Drawing shapes & text              -> DrawReferenceShapes
     STEP 3: Keyboard input & movement           -> InitPlayer / UpdatePlayer / DrawPlayer

   Read main() first to see the overall shape of the program, then read
   each function below it for the details.
   ===================================================================== */

#include "raylib.h"

/* ---- Function prototypes ----------------------------------------------
   Declaring these up top lets main() read top-to-bottom without needing
   the full function bodies in the way. Definitions (with the teaching
   comments) are below main(). */
void InitGame(int screenWidth, int screenHeight, const char *title);
void ShutdownGame(void);

void DrawReferenceShapes(void);

void InitPlayer(Vector2 *pos, float *speed, float *radius, int screenWidth, int screenHeight);
void UpdatePlayer(Vector2 *pos, float speed, float radius, int screenWidth, int screenHeight);
void DrawPlayer(Vector2 pos, float radius);

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 540;

    /* --- STEP 1: setup -------------------------------------------- */
    InitGame(screenWidth, screenHeight, "Steps 1-3 - Window, Drawing & Input");

    /* --- STEP 3: setup ------------------------------------------------- */
    Vector2 playerPos;
    float playerSpeed;
    float playerRadius;
    InitPlayer(&playerPos, &playerSpeed, &playerRadius, screenWidth, screenHeight);

    /* --- STEP 1: the game loop -----------------------------------------
       Every frame: update game state, then draw it. That's the whole
       shape of a game. */
    while (!WindowShouldClose())
    {
        /* UPDATE */
        UpdatePlayer(&playerPos, playerSpeed, playerRadius, screenWidth, screenHeight);

        /* DRAW */
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawReferenceShapes();           /* STEP 2 */
            DrawPlayer(playerPos, playerRadius); /* STEP 3 */

        EndDrawing();
    }

    /* --- STEP 1: shutdown ----------------------------------------- */
    ShutdownGame();

    return 0;
}

/* =========================================================================
   STEP 1: THE WINDOW AND THE GAME LOOP
   =========================================================================
   Every raylib program has the same basic skeleton:

     1. Open a window (once)            -> InitGame
     2. Loop until the player quits     -> the while() loop in main()
     3. Close the window (once)         -> ShutdownGame

   The loop runs many times per second (we lock it to 60 here). Each pass
   through the loop is called a "frame".
   ========================================================================= */

void InitGame(int screenWidth, int screenHeight, const char *title)
{
    /* InitWindow(width, height, title) creates the OS window and gets
       raylib's graphics system ready. Nothing else in raylib works until
       this has been called. */
    InitWindow(screenWidth, screenHeight, title);

    /* SetTargetFPS caps how many times per second the loop in main() runs.
       60 is a common target for simple 2D games. Without this, the loop
       would run as fast as the CPU allows, which wastes power and (as
       Step 3 explains) makes movement speed depend on how fast the
       computer is. */
    SetTargetFPS(60);
}

void ShutdownGame(void)
{
    /* Always close the window when you're done. If you load textures,
       sounds etc. later, they get unloaded here too - see later steps. */
    CloseWindow();
}

/* =========================================================================
   STEP 2: DRAWING SHAPES & TEXT
   =========================================================================
   raylib's screen coordinates:
     - (0, 0) is the TOP-LEFT corner of the window.
     - X increases going RIGHT.
     - Y increases going DOWN (not up! this trips up a lot of people
       coming from math class).

   Shapes are positioned differently from each other - notice the two
   different conventions below:
     - DrawRectangle(x, y, w, h, color)   -> x,y is the TOP-LEFT corner
     - DrawCircle(cx, cy, radius, color)  -> cx,cy is the CENTER
   ========================================================================= */

void DrawReferenceShapes(void)
{
    /* Rectangle: x,y is the top-left corner. */
    DrawRectangle(40, 40, 120, 60, MAROON);

    /* Circle: cx,cy is the CENTER - different from rectangles! */
    DrawCircle(300, 70, 30, DARKBLUE);

    /* Line: start point to end point. */
    DrawLine(400, 30, 550, 110, DARKGREEN);

    /* Text: x,y is the TOP-LEFT of where the text starts. */
    DrawText("Step 2: shapes are positioned differently", 40, 130, 18, GRAY);
    DrawText("Rectangles: top-left corner  |  Circles: center point", 40, 155, 16, GRAY);
}

/* =========================================================================
   STEP 3: KEYBOARD INPUT & MOVEMENT
   =========================================================================
   Two new ideas used below:

   1. IsKeyDown(key) - true for every frame a key is held down. (There's
      also IsKeyPressed, true only on the single frame a key was first
      pressed - useful for things like jumping, not for holding-to-move.)

   2. GetFrameTime() - the time, in seconds, the last frame took. We use
      this to make movement speed consistent regardless of how fast the
      game is running. If we just did "position += 5" every frame, the
      object would move twice as fast on a 120fps screen as on 60fps.
      Multiplying by delta-time (dt) fixes that: "position += speed * dt"
      always covers the same real-world distance per second, no matter
      the frame rate.
   ========================================================================= */

void InitPlayer(Vector2 *pos, float *speed, float *radius, int screenWidth, int screenHeight)
{
    /* Vector2 is a small raylib struct with x and y fields - handy for
       positions, velocities, etc. */
    pos->x = screenWidth / 2.0f;
    pos->y = screenHeight / 2.0f;
    *speed = 250.0f;   /* pixels per SECOND, not per frame */
    *radius = 20.0f;
}

void UpdatePlayer(Vector2 *pos, float speed, float radius, int screenWidth, int screenHeight)
{
    float dt = GetFrameTime();

    /* Check both WASD and arrow keys, common convention. Held keys
       add/subtract from position each frame, scaled by dt. */
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) pos->x += speed * dt;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) pos->x -= speed * dt;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) pos->y += speed * dt;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) pos->y -= speed * dt;

    /* Keep the player fully on screen by clamping its position. Without
       this you could move the circle off the edge and lose it. This
       pattern - clamp after moving - is common in simple games before
       "proper" level boundaries exist. */
    if (pos->x < radius) pos->x = radius;
    if (pos->x > screenWidth - radius) pos->x = screenWidth - radius;
    if (pos->y < radius) pos->y = radius;
    if (pos->y > screenHeight - radius) pos->y = screenHeight - radius;
}

void DrawPlayer(Vector2 pos, float radius)
{
    DrawText("Step 3: move the circle with WASD or Arrow Keys", 40, 200, 20, GRAY);
    DrawCircleV(pos, radius, MAROON); /* V = takes a Vector2 */
}

/* -----------------------------------------------------------------------
   TRY IT:
   - Step 1: change screenWidth/screenHeight, or the ClearBackground
     color, and re-run.
   - Step 2: add another shape to DrawReferenceShapes(), e.g. a second
     rectangle stacked under the first with
     DrawRectangle(40, 40 + 60 + 10, 120, 60, ...).
   - Step 3: change the speed passed into InitPlayer and feel the
     difference. Then try removing "* dt" from UpdatePlayer and compare
     behavior at SetTargetFPS(10) vs SetTargetFPS(60) - notice movement
     speed now depends on frame rate. Put "* dt" back to fix it.
   ----------------------------------------------------------------------- */
