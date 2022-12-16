#include "raylib.h"
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION // Готовая библиотека для работы со светом
#include "rlights.h"

//#if defined(PLATFORM_DESKTOP)
//#define GLSL_VERSION            330 // для пк
//
//#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
//#define GLSL_VERSION            100
//#endif
// не нужно не влияет

#define MAX_ENEMIES 10

#include "Entity.h"
#include "Ship.h"
#include "LightingShader.h"
#include "Enemy.h"
#include <vector>

Camera3D camera = { 0 };

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    int score = 0;


    InitWindow(screenWidth, screenHeight, "Asteroids bruh");
    InitAudioDevice();

    Model ship = LoadModel("resources/models/FederalCorvette/FederalCorvette.obj");

    Model enemy = LoadModel("resources/models/Asteroid/Asteroid.obj");
            
    Sound death = LoadSound("resources/sfx/death.mp3");

    Sound asteroid = LoadSound("resources/sfx/asteroid.mp3");

    Music mainTheme = LoadMusicStream("resources/sfx/mainTheme.mp3");
    mainTheme.looping = true;
    PlayMusicStream(mainTheme);

    SetMusicVolume(mainTheme, 0.6);
    SetSoundVolume(asteroid, 0.2);

    bool cooldown = false;
    bool shooting = false;
    bool gameOver = false;

    int shotTimer = 0;                                                                                                               

    Ray laser = { Vector3Zero(), Vector3Zero() }; //инициализация луча

    Enemy enemyEntity = Enemy(enemy, { 0.0f,0.0f,-100.0f }); // спавн первого моба

    std::vector<Enemy> enemies;  

    int currentEnemies = 1;
    int spawnTimer = 20;

    enemies.reserve(MAX_ENEMIES);
    enemies.emplace_back(enemyEntity);

    Ship shipEntity = Ship(ship, { 0.0f, 0.0f, 0.0f });

    LightingShader shipShader = LightingShader();
    shipEntity.shader(shipShader.rShader());
    enemyEntity.shader(shipShader.rShader());
    shipEntity.tint = WHITE;
    shipEntity.scale = 0.001f; //корабль большой пришлось уменьшить

    Texture background = LoadTexture("resources/back.png");

    // Define the camera 
    
    camera.position = Vector3 { 0.0f, 5.0f, 17.0f }; // Camera position
    camera.target = Vector3 { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;              // Camera mode type

    

    SetCameraMode(camera, CAMERA_FREE);

    // Create lights
    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, Vector3{ -2, 1, -2 }, Vector3{ 0.0f,0.0f,-10.0f }, RED, shipShader.rShader());
    lights[1] = CreateLight(LIGHT_POINT, Vector3{ 2, 1, 2 }, Vector3Zero(), WHITE, shipShader.rShader());
    lights[2] = CreateLight(LIGHT_POINT, Vector3{ -2, 1, 2 }, Vector3Zero(), WHITE, shipShader.rShader());
    lights[3] = CreateLight(LIGHT_POINT, Vector3{ 2, 1, -2 }, Vector3Zero(), WHITE, shipShader.rShader());

    SetTargetFPS(60);

    

    // Main loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);
        UpdateMusicStream(mainTheme);
        
        if (IsKeyDown(KEY_R) && (gameOver)) {
            gameOver = false;
            shipEntity.hitpoints(500);
            shipEntity.position({ 0.0f, 0.0f, 0.0f });
            enemies.clear();
            currentEnemies = 0;
            score = 0;
            PlayMusicStream(mainTheme);
        }

        if (gameOver)
        {
            BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawText("GAME OVER", 440, 260, 70, RED);
            EndDrawing();
            continue;

        }

        


        if (IsKeyDown(KEY_SPACE) && !cooldown) {
            shooting = true;
            shotTimer++;
            Vector3 tmp = shipEntity.position(); //позиция корабля во время стрельбы
            laser = Ray{ tmp, Vector3{tmp.x,tmp.y,tmp.z - 100} };
        }
        else shotTimer--, shooting = false;


        if (shotTimer < 0) shotTimer = 0;
        if (shotTimer > 150) cooldown = true;
        if (shotTimer < 50) cooldown = false;


        
        for (int i = 0; i < currentEnemies; i++) {
            enemies[i].Update();
            if (shooting)
                if (GetRayCollisionBox(laser, enemies[i].boundingBox()).hit) enemies[i].damage(GetRandomValue(10, 15)); //если попал
            if ((enemies[i].hitpoints() < 1)) {
                enemies.erase(enemies.begin() + i);
                currentEnemies--;
                score++;
                PlaySoundMulti(asteroid);
            }

            else if (enemies[i].position().z > 17) { //если астероид залетел за экран
                enemies.erase(enemies.begin() + i);
                currentEnemies--;
                score -= 2;
                if (score < 0) score = 0;
                PlaySoundMulti(asteroid);
            }

            else if (CheckCollisionBoxes(enemies[i].boundingBox(),shipEntity.boundingBox())) {
                enemies.erase(enemies.begin() + i);
                currentEnemies--;
                shipEntity.damage(50);
                if (shipEntity.hitpoints() < 1) gameOver = true, PlaySound(death), PauseMusicStream(mainTheme);
            }
        }

        if (spawnTimer < 1 && currentEnemies < MAX_ENEMIES) {
            enemies.emplace_back(enemy, Vector3{ 0.0f+GetRandomValue(-10,+10),0.0f + GetRandomValue(-6,+6),-100.0f});
            spawnTimer = 40 + GetRandomValue(-10, 40);
            currentEnemies++;
        }

        if (GetRandomValue(0, 1)) spawnTimer--;

        shipEntity.Update();
        shipShader.Update();

        // Check key inputs to enable/disable lights
        if (IsKeyPressed(KEY_Y)) { lights[3].enabled = !lights[3].enabled; }
        lights[3].position = camera.position;

        // Update light values
        for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shipShader.rShader(), lights[i]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKGRAY); //Очистка буффера
        DrawTexture(background, 0, 0, WHITE);

        BeginMode3D(camera);

        if (shooting) DrawRay(laser, RED);
        shipEntity.Draw();
        for (int i = 0; i < currentEnemies; i++) enemies[i].Draw();

        EndMode3D();


        DrawText(TextFormat("Score: %i", score), 10, 10, 30, BLUE); //полоска HP
        DrawRectangle(20, 50, 100, 10, MAROON);
        DrawRectangle(20, 50, shipEntity.hitpoints()/5, 10, SKYBLUE);
        
        DrawRectangle( 20,700, 50, 10, SKYBLUE);
        DrawRectangle( 20,700, shotTimer / 3, 10, MAROON);
        

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(ship);
    UnloadModel(enemy);
    UnloadTexture(background);
    UnloadShader(shipShader.rShader());
    UnloadMusicStream(mainTheme);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}