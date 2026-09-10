/* =====================================================================
   RAYLIB 2D TUTORIAL - STEPS 1-4
   =====================================================================
   Each step is its own Init/Update/Draw function, with teaching
   comments living inside the function they explain. main() stays a
   short, readable summary of the overall flow.

     STEP 1: The window and the game loop      -> InitGame / ShutdownGame
     STEP 2: Drawing shapes & text              -> DrawReferenceShapes
     STEP 3: Keyboard input & movement           -> UpdatePlayer
     STEP 4: Loading & drawing a real sprite     -> InitPlayer / DrawPlayer

   Read main() first to see the overall shape of the program, then read
   each function below it for the details.
   ===================================================================== */

#include "raylib.h"

/* The spritesheet is a 4x4 grid: 4 columns of frames, 4 rows (one row
   per facing direction in a typical character sheet like this one). We
   only use the very first frame (row 0, column 0) in this step - no
   animation yet, that's Step 5. */
#define SPRITE_COLUMNS 4
#define SPRITE_ROWS 4

/* ---- Function prototypes ---------------------------------------------- */
void InitGame(int screenWidth, int screenHeight, const char *title);
void ShutdownGame(Texture2D playerTexture);

void DrawReferenceShapes(void);

void InitPlayer(Vector2 *pos, float *speed, Texture2D *texture,
                 int *frameWidth, int *frameHeight,
                 int screenWidth, int screenHeight);
void UpdatePlayer(Vector2 *pos, float speed, int frameWidth, int frameHeight,
                   int screenWidth, int screenHeight);
void DrawPlayer(Vector2 pos, Texture2D texture, int frameWidth, int frameHeight);

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 540;

    /* --- STEP 1: setup -------------------------------------------- */
    InitGame(screenWidth, screenHeight, "Steps 1-4 - Window, Drawing, Input & Sprites");

    /* --- STEP 3/4: setup ------------------------------------------------
       InitPlayer now also loads the sprite texture and figures out how
       big a single frame is - see the function below for why. */
    Vector2 playerPos;
    float playerSpeed;
    Texture2D playerTexture;
    int frameWidth;
    int frameHeight;
    InitPlayer(&playerPos, &playerSpeed, &playerTexture,
               &frameWidth, &frameHeight, screenWidth, screenHeight);

    /* --- STEP 1: the game loop ------------------------------------- */
    while (!WindowShouldClose())
    {
        /* UPDATE */
        UpdatePlayer(&playerPos, playerSpeed, frameWidth, frameHeight, screenWidth, screenHeight);

        /* DRAW */
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawReferenceShapes();                                   /* STEP 2 */
            DrawPlayer(playerPos, playerTexture, frameWidth, frameHeight); /* STEP 4 */

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
   STEP 3 + STEP 4: THE PLAYER - INPUT, MOVEMENT & A REAL SPRITE
   =========================================================================
   Step 3 gave the player a position and moved it with input. Step 4
   replaces the placeholder circle with a real sprite cut out of a
   spritesheet image. Two new ideas:

   1. Texture2D / LoadTexture(path) - loads an image file from disk onto
      the GPU as a "texture" you can draw. Must happen AFTER InitWindow.

   2. Source rectangles - our spritesheet has 16 character frames packed
      into one image (4 columns x 4 rows). To draw just ONE frame, we
      tell raylib a Rectangle describing where that frame lives INSIDE
      the texture (its x, y, width, height in pixels), and raylib copies
      only that region to the screen. DrawTextureRec(texture, sourceRec,
      position, tint) does exactly this.

   Also note: DrawTextureRec draws from the TOP-LEFT corner of
   `position` - like DrawRectangle, NOT like DrawCircle's center point
   from Step 2. So playerPos now means "top-left corner of the sprite",
   not "center of the circle" like it did in Step 3.
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

    /* Start in the middle of the screen. Since position now means the
       sprite's top-left corner (see note above), we offset by half the
       frame size so the sprite is visually centered. */
    pos->x = (screenWidth / 2.0f) - (*frameWidth / 2.0f);
    pos->y = (screenHeight / 2.0f) - (*frameHeight / 2.0f);

    *speed = 250.0f; /* pixels per SECOND, not per frame */
}

void UpdatePlayer(Vector2 *pos, float speed, int frameWidth, int frameHeight,
                   int screenWidth, int screenHeight)
{
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) pos->x += speed * dt;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) pos->x -= speed * dt;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) pos->y += speed * dt;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) pos->y -= speed * dt;

    /* Clamping now uses the full frame size, not a radius, since pos is
       a top-left corner: the sprite's right edge is pos.x + frameWidth,
       and it must not go past the screen's right edge. */
    if (pos->x < 0) pos->x = 0;
    if (pos->x > screenWidth - frameWidth) pos->x = (float)(screenWidth - frameWidth);
    if (pos->y < 0) pos->y = 0;
    if (pos->y > screenHeight - frameHeight) pos->y = (float)(screenHeight - frameHeight);
}

void DrawPlayer(Vector2 pos, Texture2D texture, int frameWidth, int frameHeight)
{
    DrawText("Step 4: now a real sprite, still moved with WASD/Arrows", 40, 200, 20, GRAY);

    /* The source rectangle: which region of the spritesheet to copy.
       row 0, column 0 -> the very first frame, top-left of the sheet.
       (x, y, width, height), all in pixels within the texture.) */
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

    DrawTextureRec(texture, sourceRec, pos, WHITE);
}

/* -----------------------------------------------------------------------
   TRY IT:
   - Step 4: change the source rectangle's x in DrawPlayer, e.g.
     sourceRec.x = (float)frameWidth; - that shifts to the SECOND column
     (frame 2) of the same row. Try frameWidth * 2 and * 3 too.
   - Change sourceRec.y = (float)frameHeight; to jump to row 2 (a
     different facing direction) instead of a different column.
   - Notice DrawPlayer's tint parameter is WHITE (shows the sprite's own
     colors unchanged) - try RED or a translucent color and see what
     happens to the sprite.
   ----------------------------------------------------------------------- */
