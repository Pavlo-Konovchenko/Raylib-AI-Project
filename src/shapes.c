#include "shapes.h"
#include "raylib.h"

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
