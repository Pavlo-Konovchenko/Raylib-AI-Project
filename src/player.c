#include "player.h"

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
                   int *currentFrame, float *frameTimer, bool *isMoving,
                   int *facingRow)
{
    float dt = GetFrameTime();

    /* --- Step 3: movement ------------------------------------------------
       Still one "if" per key so diagonal movement (e.g. holding both
       Right and Down) still works - the position updates on both axes. */
    *isMoving = false;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { pos->x += speed * dt; *isMoving = true; }
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) { pos->x -= speed * dt; *isMoving = true; }
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) { pos->y += speed * dt; *isMoving = true; }
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) { pos->y -= speed * dt; *isMoving = true; }

    if (pos->x < 0) pos->x = 0;
    if (pos->x > screenWidth - frameWidth) pos->x = (float)(screenWidth - frameWidth);
    if (pos->y < 0) pos->y = 0;
    if (pos->y > screenHeight - frameHeight) pos->y = (float)(screenHeight - frameHeight);

    /* --- Step 6: which way are we facing? --------------------------------
       Unlike movement above (where multiple keys can all apply at once),
       facing needs to pick just ONE row - the sprite can't face two
       directions simultaneously. An if/else-if chain does that: the
       first held key in this list wins. That means, e.g., holding both
       Down and Right makes the sprite face down, not right - that's a
       deliberate priority choice, not a bug. Feel free to reorder it.

       We only change facing while actually moving; when idle, the
       sprite keeps facing whatever direction it last walked. */
    if (*isMoving)
    {
        if      (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) *facingRow = ROW_DOWN;
        else if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) *facingRow = ROW_UP;
        else if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) *facingRow = ROW_LEFT;
        else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) *facingRow = ROW_RIGHT;
    }

    /* --- Step 5: advance the animation frame ---------------------------- */
    if (*isMoving)
    {
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
                 int currentFrame, int facingRow)
{
    DrawText("WASD or Arrow Keys to move - sprite walks and faces the direction", 40, 200, 20, GRAY);

    /* The source rectangle: which region of the spritesheet to copy.
       x picks the column (animation frame, Step 5), y picks the row
       (facing direction, Step 6). */
    Rectangle sourceRec = { (float)(currentFrame * frameWidth), (float)(facingRow * frameHeight),
                             (float)frameWidth, (float)frameHeight };

    DrawTextureRec(texture, sourceRec, pos, WHITE);
}
