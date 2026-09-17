#include <iostream>
#include <raylib.h>
#include <cmath>

#define TILE_MAP_WIDTH 24
#define TILE_MAP_HEIGHT 24

#define TILE_SIZE 8.0f

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720

int map[TILE_MAP_WIDTH][TILE_MAP_HEIGHT]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

float posX = 128.0f, posY = 128.0f;
float viewDirX = -1.0f, viewDirY = 0.0f;
float perpX = 0.0f, perpY = 0.66f;

void render();
void update();
void raycast();

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Wolfenstein 3D RayCaster");

    while (!WindowShouldClose())
    {
        update();

        render();
    }

    return 0;
}

void render()
{
    BeginDrawing();

    ClearBackground(BLACK);

    raycast();

    EndDrawing();
}

Color getColorCode(int number)
{
    if(number == 1) return BLUE;
    if(number == 2) return GREEN;
    if(number == 3) return RED;
    if(number == 4) return WHITE;
    if(number == 5) return YELLOW;

    return BLACK;
}

void rotate(float angle)
{
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    float oldViewDirX = viewDirX;

    viewDirX = viewDirX * cosA - viewDirY * sinA;
    viewDirY = oldViewDirX * sinA + viewDirY * cosA;

    float oldPerpX = perpX;

    perpX = perpX * cosA - perpY * sinA;
    perpY = oldPerpX * sinA + perpY * cosA;
}


void update()
{
    const float MOVEMENT_SPEED = 50.0f;
    const float ROTATION_SPEED = -2.5f;

    float dt = GetFrameTime();

    if (IsKeyDown(KEY_W))
    {
        posX += viewDirX * MOVEMENT_SPEED * dt;
        posY += viewDirY * MOVEMENT_SPEED * dt;
    }

    if (IsKeyDown(KEY_S))
    {
        posX -= viewDirX * MOVEMENT_SPEED * dt;
        posY -= viewDirY * MOVEMENT_SPEED * dt;
    }

    if (IsKeyDown(KEY_D))
    {
        rotate(ROTATION_SPEED * dt);
    }

    if (IsKeyDown(KEY_A))
    {
        rotate(-ROTATION_SPEED * dt);
    }
}


void raycast()
{
    for(uint x = 0 ; x < WINDOW_WIDTH ; ++x)
    {
        float cameraX = 2 * x / float(WINDOW_WIDTH) - 1;

        float rayDirX = viewDirX + perpX * cameraX;
        float rayDirY = viewDirY + perpY * cameraX;

        int mapX = int(posX / TILE_SIZE);
        int mapY = int(posY / TILE_SIZE);

        float deltaDistX = rayDirX == 0 ? 1e30 : std::abs(1 / rayDirX);
        float deltaDistY = rayDirY == 0 ? 1e30 : std::abs(1 / rayDirY);

        float sideDistX , sideDistY;
        int stepX , stepY;

        

        if(rayDirX > 0)
        {
            stepX = 1;
            sideDistX = (((mapX + 1) * TILE_SIZE) - posX) * deltaDistX;
        }
        else
        {
            stepX = -1;
            sideDistX = (posX - (mapX * TILE_SIZE)) * deltaDistX;
        }
        
        if(rayDirY > 0)
        {
            stepY = 1;
            sideDistY = (((mapY + 1) * TILE_SIZE) - posY) * deltaDistY;
        }
        else
        {
            stepY = -1;
            sideDistY = (posY - (mapY * TILE_SIZE)) * deltaDistY;
        }

        bool hit = false;
        bool sideY = false;
        while(!hit)
        {
            if(sideDistX < sideDistY)
            {
                sideDistX += (deltaDistX * TILE_SIZE);
                mapX += stepX;
                sideY = false;
            }
            else
            {
                sideDistY += (deltaDistY * TILE_SIZE);
                mapY += stepY;
                sideY = true;
            }

            if(map[mapX][mapY] > 0) hit = true;
        }

        float perpWallDist = sideY ? (sideDistY - (deltaDistY * TILE_SIZE)) : (sideDistX - (deltaDistX * TILE_SIZE));

        int height = WINDOW_HEIGHT / perpWallDist;

        int lineStart = -height / 2 + WINDOW_HEIGHT / 2;
        int lineEnd = height / 2 + WINDOW_HEIGHT / 2;

        if(lineStart < 0) lineStart = 0;
        if(lineEnd > WINDOW_HEIGHT) lineEnd = WINDOW_HEIGHT;

        Color color = getColorCode(map[mapX][mapY]);

        if(sideY) color = Color{static_cast<unsigned char>(color.r / 2.0f) , static_cast<unsigned char>(color.g / 2.0f) , static_cast<unsigned char>(color.b / 2.0f) , color.a};

        DrawLine(x , lineStart , x , lineEnd , color);
    }
}