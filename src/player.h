#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <stdbool.h>

/* =========================================================================
   STEPS 3-6: THE PLAYER - INPUT, MOVEMENT, SPRITE, ANIMATION & FACING
   =========================================================================
   Step 3 gave the player a position and moved it with input. Step 4
   replaced a placeholder circle with a single static frame cut out of a
   spritesheet. Step 5 cycled through several frames over time so the
   sprite looks like it's walking. Step 6 picks WHICH ROW of the sheet to
   use, based on the direction being pressed, so the sprite faces the
   way it's moving.

   The spritesheet is a 4x4 grid: 4 columns (animation frames) x 4 rows
   (facing directions). Looking at the sheet, row 0 faces the viewer
   (down), row 1 shows the back of the head (up), row 2 is a left-facing
   profile, and row 3 is a right-facing profile. These row indices are
   specific to THIS spritesheet - a different sheet might order its
   directions differently, so always check visually.
   ========================================================================= */

#define SPRITE_COLUMNS 4
#define SPRITE_ROWS 4

#define ROW_DOWN  0
#define ROW_UP    1
#define ROW_LEFT  2
#define ROW_RIGHT 3

/* How many animation frames to show per second while walking. Higher =
   faster-looking legs/arms. This is separate from SetTargetFPS - that
   controls how often the whole game updates/draws; this controls how
   often the sprite's pose changes. */
#define WALK_FRAMES_PER_SECOND 8.0f

/* Loads the spritesheet and sets up starting position/speed/frame size.
   Must be called after InitGame(), since loading a texture requires the
   window/graphics context to already exist. */
void InitPlayer(Vector2 *pos, float *speed, Texture2D *texture,
                 int *frameWidth, int *frameHeight,
                 int screenWidth, int screenHeight);

/* Reads keyboard input, moves the player, keeps it on screen, and
   advances both the animation frame and the facing direction. Called
   once per frame, before drawing. */
void UpdatePlayer(Vector2 *pos, float speed, int frameWidth, int frameHeight,
                   int screenWidth, int screenHeight,
                   int *currentFrame, float *frameTimer, bool *isMoving,
                   int *facingRow);

/* Draws the correct frame (column = animation frame, row = facing
   direction) from the spritesheet at the player's current position. */
void DrawPlayer(Vector2 pos, Texture2D texture, int frameWidth, int frameHeight,
                 int currentFrame, int facingRow);

#endif /* PLAYER_H */
