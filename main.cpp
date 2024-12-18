#include <iostream>
#include <raylib.h>
#include <random>
#include <math.h>
using namespace std;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;
const int PLAYER_COUNT = 10; // of every kind
const int MAX_SPEED = 3;     // this is for 60fps, i don't think there is any need nor time to add delta time in such a small project
Color BACKGROUND_COLOR = Color{242, 238, 203, 255};
Image paper = LoadImage("assets/paper.png");
Image scissor = LoadImage("assets/scissor.png");
Image stone = LoadImage("assets/stone.png");

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
private:
    pair<int, int> velocity; // vector
    pair<int, int> position; // point
    Texture2D texture;
    GameObjectType type;
    const int radius = 25;

public:
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
};
int main()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Showdown");
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
    CloseWindow();
}