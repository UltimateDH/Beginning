#include<iostream>
#include<raylib.h>
#include<raymath.h>
using std::string;
class sound{
public:
    Sound paddlehit;
    Sound gameover;
    void play(){
        PlaySound(paddlehit);
    }
    void un(){
        UnloadSound(paddlehit);
    }
    void over(){
        PlaySound(gameover);
    }
};
const int winning_score=7;
int playerscore=0;
int cpuscore=0;
enum Gamestate{Playing, Game_Over};
Gamestate currentstate= Playing;

class Ball: public sound
{
public:
    float x,y;
    int speed_x,speed_y;
    int radius;

    void Draw(){
        DrawCircle(x,y,radius,RED);
    }

    //For Sound
    void hit(){
        PlaySound(paddlehit);
    }
    
    void Update(){
        x+=speed_x;
        y+=speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <=0)
        {
            speed_y*=-1;
            hit();
        }
        if(x + radius >= GetScreenWidth())
        {
            cpuscore++;
            ResetBall();
        } 
        if(x - radius <=0)
        {
            playerscore++;
            ResetBall();
        }
        
    }

    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};
        speed_x*=speed_choices[GetRandomValue(0,1)];
        speed_y*=speed_choices[GetRandomValue(0,1)];
    }
};

class paddle{
protected:
    void LimitedMovement(){
        if(y <= 0){
            y=0;
        }
        if(y >= GetScreenHeight()-height){
            y=GetScreenHeight()-height;
        }
    }
public:
    float x,y;
    float width,height;
    int speed_y;

    void Draw(){
       DrawRectangle(x,y,width,height,BLACK);
        
    }

    void Update(){
        
        if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
            y-=speed_y;
        }
        if(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
            y+=speed_y;
        }
        if(y <= 0){
            y=0;
        }
        if(y >= GetScreenHeight()-height){
            y=GetScreenHeight()-height;
        }
        LimitedMovement();
    }
};

class CpuPaddle: public paddle{
public:
    void Update(int ball_y){
        if(y+height/2 > ball_y){
        y-=speed_y;
        }
        if(y+height/2 <= ball_y){
            y+=speed_y;
        }
        LimitedMovement();
    }
    
};

Ball ball;
paddle player;
CpuPaddle cpu;
sound test;

int main()
{      
    int n,m;
    const int screen_width = 1280;
    const int screen_height = 800;
    
    InitWindow(screen_width,screen_height,"pong.exe");
    SetTargetFPS(60);

    InitAudioDevice();
    SetSoundVolume(test.paddlehit,1.0f);
    SetSoundVolume(test.gameover,1.0f);
    test.paddlehit=LoadSound("pong.wav");
    ball.paddlehit=LoadSound("pong.wav");
    test.gameover=LoadSound("gameover.wav");
    n=MeasureText("Press ENTER to restart or ESCAPE to close the program",20);
    m=MeasureText("Game Over",40);

    
    
    ball.x=screen_width/2;
    ball.y=screen_height/2;
    ball.radius=20;
    ball.speed_x=7;
    ball.speed_y=7;

    player.width=15;
    player.height=120;
    player.x=screen_width-player.width;
    player.y=screen_height/2-player.height/2;
    player.speed_y=6;

    cpu.width=15;
    cpu.height=120;
    cpu.x=0;
    cpu.y=screen_height/2-cpu.height/2;
    cpu.speed_y=6;



    // Game loop
    while(!WindowShouldClose())
    {

    
    if(currentstate==Playing){


    // Checking for collisions
    if(CheckCollisionCircleRec(Vector2{ball.x,ball.y}, ball.radius, Rectangle{player.x,player.y,player.width,player.height}))
    {
        ball.speed_x*=-1;
        test.play();
    }
    if(CheckCollisionCircleRec(Vector2{ball.x,ball.y}, ball.radius, Rectangle{cpu.x,cpu.y,cpu.width,cpu.height}))
    {
        ball.speed_x*=-1;
        test.play();
    }

    // Updating positions
    ball.Update();
    player.Update();
    cpu.Update(ball.y);

    // Drawing
    BeginDrawing();
    ClearBackground(WHITE);
    DrawLine(screen_width/2,0,screen_width/2,screen_height,BLACK);
    ball.Draw();
    cpu.Draw();
    player.Draw();
    DrawText(TextFormat("%i",cpuscore),screen_width/4-20,20,80,BLACK);
    DrawText(TextFormat("%i",playerscore),3*screen_width/4-20,20,80,BLACK);
    if(cpuscore>=winning_score || playerscore>=winning_score){
        currentstate=Game_Over;
        test.over();
    }
    }
    else if(currentstate==Game_Over){
        ClearBackground(WHITE);
        DrawText("Game Over",(GetScreenWidth()-m)/2,(GetScreenHeight()/2)-55,40,RED);
        DrawText("Press ENTER to restart or BACKSPACE to close the program",(GetScreenWidth()-n)/2,(GetScreenHeight()/2)-20,20,BLACK);
        if(IsKeyPressed(KEY_ENTER)){
            //Reset Game
            cpuscore=0; 
            playerscore=0;
            ball.ResetBall();
            currentstate=Playing;

        }
        if(IsKeyPressed(KEY_BACKSPACE)){
            break;
        }
    }
    EndDrawing();
    
    
    }  
    UnloadSound(test.paddlehit);
    CloseAudioDevice();
    CloseWindow();

}   