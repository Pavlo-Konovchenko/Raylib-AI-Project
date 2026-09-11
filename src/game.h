#ifndef GAME_H
#define GAME_H

#include "raylib.h"

/* =========================================================================
   STEP 1: THE WINDOW AND THE GAME LOOP
   =========================================================================
   Every raylib program has the same basic skeleton:

     1. Open a window (once)            -> InitGame
     2. Loop until the player quits     -> the while() loop in main()
     3. Close the window (once)         -> ShutdownGame

   The loop runs many times per second (main.c locks it to 60). Each pass
   through the loop is called a "frame".
   ========================================================================= */

/* Opens the window and starts the frame-rate cap. Must be called before
   anything else in raylib - including loading the player's texture in
   player.c, which is why InitGame() runs first in main(). */
void InitGame(int screenWidth, int screenHeight, const char *title);

/* Closes the window. Takes the player's texture so it can be unloaded
   from the GPU first - see the comment in game.c for why the order
   matters. */
void ShutdownGame(Texture2D playerTexture);

#endif /* GAME_H */
