#include <iostream>
#include <raylib.h>
#include <random>
#include <math.h>
using namespace std;

pair<int, int> createRandomPoint(int minX, int maxX,int minY,int maxY)
{
    // Generating a random coordinate for the point to display
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disX(minX, maxX);
    uniform_int_distribution<> disY(minY, maxY);
    int x_cordi = disX(gen);
    int y_cordi = disY(gen);
    return {x_cordi, y_cordi};
}
bool isInsideCircle(pair<int, int> &coordi, pair<int, int> origin, int radius)
{
    pair<int, int> shiftedOrigin = {origin.first - coordi.first, origin.second - coordi.second};
    // checking the basic condition of x^2 + y^2 < r^2 for a circle (i am here taking on the circle as outside circle too)
    if (radius >= sqrt(shiftedOrigin.first*shiftedOrigin.first + shiftedOrigin.second*shiftedOrigin.second)){
        return true;
    }
    return false;
}
int main()
{
    const int SCREEN_WIDTH = 650;
    const int SCREEN_HEIGHT = 650;
    const int SQUARE_SIZE = 600;
    int insideDots = 0;
    int outsideDots = 0;
    int totalDots = 0;
    float mostAccuratePi = 0;
    float leastError = 100;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Monte Carlo Simulation");
    SetTargetFPS(120);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawRectangleLines((SCREEN_WIDTH - SQUARE_SIZE) / 2, (SCREEN_HEIGHT - SQUARE_SIZE) / 2, SQUARE_SIZE, SQUARE_SIZE, WHITE);
        pair<int, int> newPoint = createRandomPoint((SCREEN_WIDTH - SQUARE_SIZE) / 2, (SCREEN_WIDTH + SQUARE_SIZE) / 2,(SCREEN_HEIGHT - SQUARE_SIZE) / 2, (SCREEN_HEIGHT + SQUARE_SIZE) / 2);
        totalDots++;
        bool isInside = isInsideCircle(newPoint,{SCREEN_WIDTH/2, SCREEN_HEIGHT/2},SQUARE_SIZE/2);
        if (isInside){
            insideDots++;
        }
        else{
            outsideDots++;
        }
        DrawCircle(newPoint.first, newPoint.second, 1, isInside? RED : BLUE);
        cout<<"--------------------------------------------"<<endl;
        cout<<"Total Dots: "<<totalDots<<endl;
        cout<<"Dots Inside Circle: "<<insideDots<<endl;
        cout<<"Dots Outside Circle: "<<outsideDots<<endl;
        float calcPi = 4*(float)insideDots/(float)totalDots;
        float error = abs(PI-calcPi)*100/PI;
        cout<<"Estimated Pi: "<<calcPi<<endl;
        cout<<"Error:" <<error<<"%"<<endl;
        if (error<leastError){
            leastError = error;
            mostAccuratePi = calcPi;
        }
        cout<<"--------------------------------------------"<<endl;
        EndDrawing();
    }
    cout<<"Most Accurate PI during Simulation was "<<mostAccuratePi<<endl;
    cout<<"With Error Percentage of "<<leastError<<"%"<<endl;
    CloseWindow();
}