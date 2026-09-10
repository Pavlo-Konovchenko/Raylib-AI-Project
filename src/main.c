#include "raylib.h"

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "Raylib Game Template");
    SetTargetFPS(60);

    // Simple player state
    Vector2 playerPos = { screenWidth / 2.0f, screenHeight / 2.0f };
    float playerSpeed = 250.0f; // pixels per second
    float playerRadius = 20.0f;

    while (!WindowShouldClose()) // loops until window close / ESC
    {
        // ---- Update ----
        float dt = GetFrameTime();

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) playerPos.x += playerSpeed * dt;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) playerPos.x -= playerSpeed * dt;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) playerPos.y += playerSpeed * dt;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) playerPos.y -= playerSpeed * dt;

        // Keep player on screen
        if (playerPos.x < playerRadius) playerPos.x = playerRadius;
        if (playerPos.x > screenWidth - playerRadius) playerPos.x = screenWidth - playerRadius;
        if (playerPos.y < playerRadius) playerPos.y = playerRadius;
        if (playerPos.y > screenHeight - playerRadius) playerPos.y = screenHeight - playerRadius;

        // ---- Draw ----
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("Raylib Game Template", 20, 20, 24, DARKGRAY);
            DrawText("Move with WASD or Arrow Keys", 20, 50, 16, GRAY);
            DrawFPS(screenWidth - 90, 10);

            DrawCircleV(playerPos, playerRadius, MAROON);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
