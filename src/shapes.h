#ifndef SHAPES_H
#define SHAPES_H

/* =========================================================================
   STEP 2: DRAWING SHAPES & TEXT
   =========================================================================
   raylib's screen coordinates:
     - (0, 0) is the TOP-LEFT corner of the window.
     - X increases going RIGHT.
     - Y increases going DOWN (not up! this trips up a lot of people
       coming from math class).

   Shapes are positioned differently from each other:
     - DrawRectangle(x, y, w, h, color)   -> x,y is the TOP-LEFT corner
     - DrawCircle(cx, cy, radius, color)  -> cx,cy is the CENTER
   ========================================================================= */

/* Draws a small set of reference shapes/text demonstrating the ideas
   above. Not part of the "real" game - just a visual cheat-sheet left
   on screen from Step 2. Safe to delete once you don't need the
   reminder anymore. */
void DrawReferenceShapes(void);

#endif /* SHAPES_H */
