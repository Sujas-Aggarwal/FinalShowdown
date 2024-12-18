#include <iostream>
#include <raylib.h>
#include <random>
#include <math.h>
using namespace std;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;
const int PLAYER_COUNT = 1; // of every kind
const int MAX_SPEED = 3;     // this is for 60fps, i don't think there is any need nor time to add delta time in such a small project
Color BACKGROUND_COLOR = Color{242, 238, 203, 255};
Image paper = LoadImage("assets/paper.png");
Image scissor = LoadImage("assets/scissor.png");
Image stone = LoadImage("assets/stone.png");
Sound paperWin = LoadSound("assets/paper.wav");
Sound scissorWin = LoadSound("assets/scissor.wav");
Sound stoneWin = LoadSound("assets/stone.wav");
enum class GameObjectType
{
    PAPER,
    SCISSOR,
    STONE
};
pair<int, int> genRandVal(int minX, int maxX, int minY, int maxY)
{
    // Generating a random coordinate for the point to display
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disX(minX, maxX);
    uniform_int_distribution<> disY(minY, maxY);
    int x_cordi = disX(gen);
    if (x_cordi == 0)
        x_cordi = disX(gen);
    int y_cordi = disY(gen);
    if (y_cordi == 0)
        y_cordi = disY(gen);
    return {x_cordi, y_cordi};
}
class GameObjects
{
public:
    pair<int, int> velocity; // vector
    pair<int, int> position; // point
    Texture2D texture;
    GameObjectType type;
    const int radius = 25;
    GameObjects(GameObjectType type)
    {
        this->velocity = genRandVal(-MAX_SPEED, MAX_SPEED, -MAX_SPEED, MAX_SPEED);
        this->position = genRandVal(this->radius, SCREEN_WIDTH - this->radius, this->radius, SCREEN_HEIGHT - this->radius);
        this->type = type;
        switch (type)
        {
        case GameObjectType::PAPER:
            if (paper.height != this->radius * 2 || paper.width != this->radius * 2)
            {
                ImageResize(&paper, this->radius * 2, this->radius * 2);
                ImageRotateCCW(&paper);
            }
            this->texture = LoadTextureFromImage(paper);
            break;
        case GameObjectType::SCISSOR:
            if (scissor.height != this->radius * 2 || scissor.width != this->radius * 2)
            {
                ImageResize(&scissor, this->radius * 2, this->radius * 2);
                ImageRotateCCW(&scissor);
            }
            this->texture = LoadTextureFromImage(scissor);
            break;
        case GameObjectType::STONE:
            if (stone.height != this->radius * 2 || stone.width != this->radius * 2)
            {
                ImageResize(&stone, this->radius * 2, this->radius * 2);
                ImageRotateCCW(&stone);
            }
            this->texture = LoadTextureFromImage(stone);
            break;
        }
    }
    ~GameObjects()
    {
        UnloadTexture(texture);
    }
    void update()
    {
        position.first += velocity.first;
        position.second += velocity.second;
        if (position.first <= this->radius || position.first >= SCREEN_WIDTH - this->radius)
        {
            velocity.first *= -1;
        }
        if (position.second <= this->radius || position.second >= SCREEN_HEIGHT - this->radius)
        {
            velocity.second *= -1;
        }
    }
    void draw()
    {
        DrawTexture(texture, position.first - this->radius, position.second - this->radius, WHITE);
    }
    static void collision(GameObjects *a,GameObjects *b)
    {
        if (a->type == GameObjectType::PAPER && b->type == GameObjectType::STONE)
        {
            b->type = GameObjectType::PAPER;
            b->texture = LoadTextureFromImage(paper);
            PlaySound(paperWin);
        }
        else if (a->type == GameObjectType::PAPER && b->type == GameObjectType::SCISSOR)
        {
            a->type = GameObjectType::SCISSOR;
            a->texture = LoadTextureFromImage(scissor);
            PlaySound(scissorWin);
        }
        else if (a->type == GameObjectType::STONE && b->type == GameObjectType::PAPER)
        {
            a->type = GameObjectType::PAPER;
            a->texture = LoadTextureFromImage(paper);
            PlaySound(paperWin);
        }
        else if (a->type == GameObjectType::STONE && b->type == GameObjectType::SCISSOR)
        {
            b->type = GameObjectType::STONE;
            b->texture = LoadTextureFromImage(stone);
            PlaySound(stoneWin);
        }
        else if (a->type == GameObjectType::SCISSOR && b->type == GameObjectType::PAPER)
        {
            b->type = GameObjectType::SCISSOR;
            b->texture = LoadTextureFromImage(scissor);
            PlaySound(scissorWin);
        }
        else if (a->type == GameObjectType::SCISSOR && b->type == GameObjectType::STONE)
        {
            a->type = GameObjectType::STONE;
            a->texture = LoadTextureFromImage(stone);
            PlaySound(stoneWin);
        }
    }
};
int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Showdown");
    InitAudioDevice();
    SetMasterVolume(1.0f);
    SetTargetFPS(60);

    vector<GameObjects *> gameObjects(3 * PLAYER_COUNT);
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        gameObjects[i] = new GameObjects(GameObjectType::PAPER);
    }
    for (int i = PLAYER_COUNT; i < 2 * PLAYER_COUNT; i++)
    {
        gameObjects[i] = new GameObjects(GameObjectType::STONE);
    }
    for (int i = 2 * PLAYER_COUNT; i < 3 * PLAYER_COUNT; i++)
    {
        gameObjects[i] = new GameObjects(GameObjectType::SCISSOR);
    }
    while (!WindowShouldClose())
    {
        // input toh kuch h nahi since simulation h more or less
        // update
        for (int i = 0; i < 3 * PLAYER_COUNT; i++)
        {
            gameObjects[i]->update();
        }
        //check collision
        for (int i = 0; i < 3 * PLAYER_COUNT; i++)
        {
            for (int j = i + 1; j < 3 * PLAYER_COUNT; j++)
            {
                bool isColliding = CheckCollisionCircles(Vector2{(float)gameObjects[i]->position.first, (float)gameObjects[i]->position.second}, gameObjects[i]->radius, Vector2{(float)gameObjects[j]->position.first, (float)gameObjects[j]->position.second}, gameObjects[j]->radius);
                if (isColliding)
                    GameObjects::collision(gameObjects[i], gameObjects[j]);
            }
        }
        BeginDrawing();
        gameObjects[0]->update();
        ClearBackground(BACKGROUND_COLOR);
        for (int i = 0; i < 3 * PLAYER_COUNT; i++)
        {
            gameObjects[i]->draw();
        }
        gameObjects[0]->draw();
        // draw
        EndDrawing();
    }
    UnloadImage(paper);
    UnloadImage(scissor);
    UnloadImage(stone);
    UnloadSound(paperWin);
    UnloadSound(scissorWin);
    UnloadSound(stoneWin);
    CloseAudioDevice();
    CloseWindow();
}