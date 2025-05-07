#include<iostream>
#include<raylib.h>
int main(){
    
    int ballx=400;
    int bally=400;
    InitWindow(800,800,"Ball game");
    SetTargetFPS(60);

    //Game loop
    while(!WindowShouldClose()){

        //1. Event handling
        if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        {
            ballx+=3;
        }
        else if(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            bally+=3;
        }
        else if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            bally-=3;
        }
        else if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            ballx-=3;
        }

        //2. Updating Positions
        

        //3. Drawing elements
        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircle(ballx,bally,15,BLACK);

        EndDrawing();


    }



    CloseWindow();
}