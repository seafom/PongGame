#include "raylib.h"
#include <iostream>

using namespace std;

enum class Scene
{
    MENU,
    GAME,
    SCORE
};

// Player Paddle
class Paddle
{
public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void MoveUp()
    {
        y -= speed;
    }

    void MoveDown()
    {
        y += speed;
    }
};

// CPU Paddle
class CpuPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        // CPU moves towards the ball
        if (y + height / 2 < ball_y)
            y += speed;
        else if (y + height / 2 > ball_y)
            y -= speed;
    }
};


int main()
{
    int screen_width = 700;
    int screen_height = 700;

    InitWindow(screen_width, screen_height, "Pong Game");
    SetTargetFPS(60);

    Scene currentScene = Scene::MENU;

    int ballX = 200;
    int ballY = 200;
    float radiusball = 20;

    Paddle player;
    CpuPaddle cpu;

    // Initialize player and CPU paddles
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    // Ball Speed
    int ballVelocityX = 6;
    int ballVelocityY = 6;

    // Player Score
    int playerScore = 0;
    // CPU Score
    int cpuScore = 0;

    Rectangle playerPaddle;
    Rectangle cpuPaddle;

    while (!WindowShouldClose())
    {
        switch (currentScene)
        {
        case Scene::MENU:

            // MENU
            ClearBackground(PINK);
            DrawText(TextFormat("Pong Game"), screen_width / 2.5 - 50, 120, 50, WHITE);
            DrawText("Press Press F To Play", screen_width / 2 - MeasureText("Press F To Play", 30) / 2, screen_height / 2, 20, WHITE);

            if (IsKeyPressed(KEY_F))
            {
                currentScene = Scene::GAME;
            }
            break;


            // GAME
        case Scene::GAME:
            ballX += ballVelocityX;
            ballY += ballVelocityY;

            playerPaddle = { player.x, player.y, player.width, player.height };
            cpuPaddle = { cpu.x, cpu.y, cpu.width, cpu.height };

            // Collision For The Paddles
            if (CheckCollisionCircleRec({ ballX + radiusball, ballY + radiusball }, radiusball, playerPaddle) ||
                CheckCollisionCircleRec({ ballX + radiusball, ballY + radiusball }, radiusball, cpuPaddle))
            {
                ballVelocityX *= -1;
            }


            // RIGHT PADDLE For Player
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
                player.MoveUp();
            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
                player.MoveDown();

            cpu.Update(ballY);


            // Check Collision For Walls
            // Cpu wins and the ball goes on the right then cpuscore++;
            if ((ballX + radiusball >= screen_width))
            {
                ballX = screen_width / 2;
                ballY = screen_height / 2;
                ballVelocityX  ++;
                ballVelocityY ++;
                cpuScore++;
            }

            // Player wins and the ball goes on the left then playerscore++;
            if (ballX - radiusball <= 0)
            {
                ballX = screen_width / 2;
                ballY = screen_height / 2;
                ballVelocityX ++;
                ballVelocityY ++;
                playerScore++;
            }

            if ((ballY + radiusball >= screen_height || ballY - radiusball <= 0))
            {
                ballVelocityY *= -1;
            }



            // Draw game elements
            BeginDrawing();
            ClearBackground(PINK);
            DrawCircle(ballX, ballY, radiusball, RED);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            player.Draw();
            cpu.Draw();



            // Display SCORES
            DrawText(TextFormat("%i", cpuScore), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore), 3 * screen_width / 4 - 20, 20, 80, WHITE);





        case Scene::SCORE:
            // SCORE SCREEN

            if (playerScore >= 10)
            {
                ClearBackground(PINK);
                DrawText("Player wins !", screen_width / 2 - MeasureText("Player wins !", 30) / 2, screen_height / 2, 30, WHITE);
                DrawText("Press T to return to menu or Space to replay", screen_width / 2 - MeasureText("Press T to return to menu or Space to replay", 20) / 2, screen_height / 2 + 40, 20, WHITE);

            }
            else if (cpuScore >= 10)
            {
                ClearBackground(PINK);
                DrawText("CPU wins !", screen_width / 2 - MeasureText("CPU wins !", 30) / 2, screen_height / 2, 30, WHITE);
                DrawText("Press T to return to menu or Space to replay", screen_width / 2 - MeasureText("Press T to return to menu or Space to replay", 20) / 2, screen_height / 2 + 40, 20, WHITE);

            }

            if (IsKeyPressed(KEY_SPACE))
            {
                currentScene = Scene::GAME;

                // Reset
                playerScore = 0;
                cpuScore = 0;
                ballX = screen_width / 2;
                ballY = screen_height / 2;
                ballX += ballVelocityX;
                ballY += ballVelocityY;
            }
            else if (IsKeyPressed(KEY_T))
            {
                currentScene = Scene::MENU;

                // Reset 
                playerScore = 0;
                cpuScore = 0;
                ballX = screen_width / 2;
                ballY = screen_height / 2;
               
            }
            break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}