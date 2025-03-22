#include <raylib.h>
#include "geo.h"
#include "world.h"
#include "player.h"
#include "defines.h"

PlayerPrey* players[2];
float dt;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    // Init Players
    Circle c1, c2; 
    c1.radius = 30;
    c2.radius = 30;
    c1.pos.x = 200; 
    c1.pos.y = 200;
    c2.pos.x = 500;
    c2.pos.y = 200;

    players[0] = p_player_prey_create(0, 200, &c1);
    players[1] = p_player_prey_create(0, 200, &c2);

    // -------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        dt = GetFrameTime();
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawCircle((int)players[0]->hitbox.pos.x, (int)players[0]->hitbox.pos.y, players[0]->hitbox.radius, RED);
            DrawCircle((int)players[1]->hitbox.pos.x, (int)players[1]->hitbox.pos.y, players[1]->hitbox.radius, DARKBLUE);

            Position cursor;
            cursor.x = (float)GetMouseX();
            cursor.y = (float)GetMouseY();
            //trucs
    
            if(IsKeyDown(KEY_W))
                p_player_prey_move(players[0],&cursor);
            //p_player_prey_move(players[1],&cursor,dt);

            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    p_player_prey_free(players[0]);
    p_player_prey_free(players[1]);   
    //--------------------------------------------------------------------------------------

    return 0;
}
