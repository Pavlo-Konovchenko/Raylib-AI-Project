/* =====================================================================
   RAYLIB 2D TUTORIAL - STEPS 1-5
   =====================================================================
   Each step is its own Init/Update/Draw function, with teaching
   comments living inside the function they explain. main() stays a
   short, readable summary of the overall flow.

     STEP 1: The window and the game loop      -> InitGame / ShutdownGame
     STEP 2: Drawing shapes & text              -> DrawReferenceShapes
     STEP 3: Keyboard input & movement           -> UpdatePlayer
     STEP 4: Loading & drawing a real sprite     -> InitPlayer / DrawPlayer
     STEP 5: Animating the sprite                -> UpdatePlayer / DrawPlayer

   Read main() first to see the overall shape of the program, then read
   each function below it for the details.
   ===================================================================== */

#include "raylib.h"
#include <stdbool.h>

/* The spritesheet is a 4x4 grid: 4 columns of frames, 4 rows (one row
   per facing direction in a typical character sheet like this one). We
   animate across the 4 columns of a single row - no direction switching
   yet, that would be a natural next step. */
#define SPRITE_COLUMNS 4
#define SPRITE_ROWS 4

/* How many animation frames to show per second while walking. Higher =
   faster-looking legs/arms. This is separate from SetTargetFPS - that
   controls how often the whole game updates/draws; this controls how
   often the sprite's pose changes. */
#define WALK_FRAMES_PER_SECOND 8.0f

/* ---- Function prototypes ---------------------------------------------- */
void InitGame(int screenWidth, int screenHeight, const char *title);
void ShutdownGame(Texture2D playerTexture);

void DrawReferenceShapes(void);

void InitPlayer(Vector2 *pos, float *speed, Texture2D *texture,
                 int *frameWidth, int *frameHeight,
                 int screenWidth, int screenHeight);
void UpdatePlayer(Vector2 *pos, float speed, int frameWidth, int frameHeight,
                   int screenWidth, int screenHeight,
                   int *currentFrame, float *frameTimer, bool *isMoving);
void DrawPlayer(Vector2 pos, Texture2D texture, int frameWidth, int frameHeight,
                 int currentFrame);

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 540;

    /* --- STEP 1: setup -------------------------------------------- */
    InitGame(screenWidth, screenHeight, "Steps 1-5 - Window, Drawing, Input & Animated Sprite");

    /* --- STEP 3/4: setup ------------------------------------------------
       InitPlayer loads the sprite texture and figures out how big a
       single frame is - see the function below for why. */
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

    /* --- STEP 1: the game loop ------------------------------------- */
    while (!WindowShouldClose())
    {
        /* UPDATE */
        UpdatePlayer(&playerPos, playerSpeed, frameWidth, frameHeight, screenWidth, screenHeight,
                     &currentFrame, &frameTimer, &isMoving);

        /* DRAW */
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawReferenceShapes();                                              /* STEP 2 */
            DrawPlayer(playerPos, playerTexture, frameWidth, frameHeight, currentFrame); /* STEP 4/5 */

        EndDrawing();
    }

    /* --- STEP 1/4: shutdown ----------------------------------------- */
    ShutdownGame(playerTexture);

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
   STEP 3 + 4 + 5: THE PLAYER - INPUT, MOVEMENT, SPRITE & ANIMATION
   =========================================================================
   Step 3 gave the player a position and moved it with input. Step 4
   replaced the placeholder circle with a single static frame cut out of
   a spritesheet. Step 5 (this one) cycles through several frames over
   time so the sprite looks like it's walking instead of sliding.

   The core animation idea, "frame timer":
     - Keep a running total of elapsed time (frameTimer).
     - Every frame, add how long that frame took (GetFrameTime()).
     - Once the total passes some threshold (1 / frames-per-second),
       reset it back to zero and move on to the next animation frame.
     - Wrap back to frame 0 after the last one (modulo `%`).

   This is the same delta-time idea from Step 3's movement, just used to
   pace animation instead of position.
   ========================================================================= */

void InitPlayer(Vector2 *pos, float *speed, Texture2D *texture,
                 int *frameWidth, int *frameHeight,
                 int screenWidth, int screenHeight)
{
    /* Load the spritesheet. The path is relative to the .exe, which is
       why CMakeLists.txt copies the assets/ folder next to it after
       every build. */
    *texture = LoadTexture("assets/player_spritesheet.png");

    /* Figure out the size of ONE frame by dividing the whole sheet by
       how many columns/rows it has. This way the code doesn't need the
       exact pixel size hardcoded - it adapts to the image. */
    *frameWidth = texture->width / SPRITE_COLUMNS;
    *frameHeight = texture->height / SPRITE_ROWS;

    /* Start in the middle of the screen. Position means the sprite's
       top-left corner (DrawTextureRec draws from there, not the
       center), so we offset by half the frame size to visually center
       it. */
    pos->x = (screenWidth / 2.0f) - (*frameWidth / 2.0f);
    pos->y = (screenHeight / 2.0f) - (*frameHeight / 2.0f);

    *speed = 250.0f; /* pixels per SECOND, not per frame */
}

void UpdatePlayer(Vector2 *pos, float speed, int frameWidth, int frameHeight,
                   int screenWidth, int screenHeight,
                   int *currentFrame, float *frameTimer, bool *isMoving)
{
    float dt = GetFrameTime();

    /* --- Step 3: movement, unchanged ----------------------------------- */
    *isMoving = false;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { pos->x += speed * dt; *isMoving = true; }
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) { pos->x -= speed * dt; *isMoving = true; }
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) { pos->y += speed * dt; *isMoving = true; }
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) { pos->y -= speed * dt; *isMoving = true; }

    if (pos->x < 0) pos->x = 0;
    if (pos->x > screenWidth - frameWidth) pos->x = (float)(screenWidth - frameWidth);
    if (pos->y < 0) pos->y = 0;
    if (pos->y > screenHeight - frameHeight) pos->y = (float)(screenHeight - frameHeight);

    /* --- Step 5: advance the animation frame ---------------------------- */
    if (*isMoving)
    {
        /* Accumulate time, and every time we cross the "seconds per
           frame" threshold, move to the next frame and carry over any
           leftover time (rather than resetting to exactly 0, which
           would slowly drift out of sync with the real elapsed time). */
        *frameTimer += dt;
        float secondsPerFrame = 1.0f / WALK_FRAMES_PER_SECOND;
        if (*frameTimer >= secondsPerFrame)
        {
            *frameTimer -= secondsPerFrame;
            *currentFrame = (*currentFrame + 1) % SPRITE_COLUMNS;
        }
    }
    else
    {
        /* Standing still: show a fixed idle pose (frame 0) instead of
           whatever frame the walk cycle stopped on. */
        *currentFrame = 0;
        *frameTimer = 0.0f;
    }
}

void DrawPlayer(Vector2 pos, Texture2D texture, int frameWidth, int frameHeight,
                 int currentFrame)
{
    DrawText("Step 5: walk - the sprite now animates while moving", 40, 200, 20, GRAY);

    /* The source rectangle: which region of the spritesheet to copy.
       x shifts across columns (the animation frame), y picks the row
       (row 0 here - a fixed facing direction for now). */
    Rectangle sourceRec = { (float)(currentFrame * frameWidth), 0.0f,
                             (float)frameWidth, (float)frameHeight };

    DrawTextureRec(texture, sourceRec, pos, WHITE);
}

/* -----------------------------------------------------------------------
   TRY IT:
   - Change WALK_FRAMES_PER_SECOND to 2 (slow, sluggish) or 16 (fast,
     twitchy) and see how it changes the feel of the walk.
   - In DrawPlayer, hardcode sourceRec.y to frameHeight (row 1) or
     frameHeight * 2 (row 2) to preview a different row of the sheet.
   - Natural next step (not included here): pick the row based on which
     direction key is held, so the sprite actually faces the direction
     it's moving instead of always using row 0.
   ----------------------------------------------------------------------- */
