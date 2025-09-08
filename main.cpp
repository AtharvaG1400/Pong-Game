#include <iostream>
#include <raylib.h>

using namespace std;

Color BackgroundDark   = Color{ 10, 10, 30, 255 };
Color SidePanelDark    = Color{ 20, 20, 50, 255 };
Color NeonCyan         = Color{ 0, 255, 200, 255 };
Color NeonMagenta      = Color{ 255, 50, 180, 255 };
Color NeonPurple       = Color{ 150, 80, 255, 255 };
Color SoftWhite        = Color{ 240, 240, 255, 255 };

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, NeonMagenta);
    }
    
    void ResetBall() {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth() || x - radius <= 0) {
            speed_x *= -1;
        }
        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }
        if (x - radius <= 0) {
            player_score++;
            ResetBall();
        }
    }
};

class Paddle {
public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, NeonCyan);
    }

    void Update() {
        if (IsKeyDown(KEY_UP) && y > 20) {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN) && y + height < GetScreenHeight() - 20) {
            y += speed;
        }
    }
};

class CPUPaddle : public Paddle {
public:
    void Update(int ball_y) {
        if (y + height/2 < ball_y && y + height < GetScreenHeight()) {
            y += speed;
        }
        if (y + height/2 > ball_y && y > 0) {
            y -= speed;
        }
    }
};

Ball ball;
Paddle player;
CPUPaddle cpu;

int main () {

    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong Game");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;

    // Game Loop
    while (!WindowShouldClose()) {
        BeginDrawing();

        // Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Collision Checking
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
            ball.x = player.x - ball.radius;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
            ball.x = cpu.x + cpu.width + ball.radius;
        }

        //Drawing
        ClearBackground(BackgroundDark);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, SidePanelDark);
        DrawCircle(screen_width/2, screen_height/2, 150, NeonPurple);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, SoftWhite);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, SoftWhite);
        DrawText(TextFormat("%i", player_score), 3 * screen_width/4 - 20, 20, 80, SoftWhite);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}