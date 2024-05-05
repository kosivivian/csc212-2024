#include "PyramidNoOOP.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <SDL.h>

struct Point2D { float x; float y; };
struct Point3D { float x; float y; float z; };
struct Edge { int start; int end; };

int WindowSizeX;
int WindowSizeY;
SDL_Renderer* renderer;
SDL_Window* window;
float FL = 5;
float rotation = 0;
float DeltaTime;
float scaledValue = 2;

int iterations = 0;


//Creates an array of points
std::vector<Point3D> points2{
         Point3D{ -1, -1, 0 }, Point3D{ 1, 1, 0 },
         Point3D{ 1,  -1, 0 }, Point3D{ -1,  1, 0 },
         Point3D{ 0, 0.5, 4 }
};

//creates an array of edges (This hold indices referencing to the points array)
std::vector<Edge> edges2{
         Edge{ 0, 2 },
         Edge{ 0, 3 },
         Edge{ 0, 4 },
         Edge{ 1, 3 },
         Edge{ 1, 2 },
         Edge{ 1, 4 },
         Edge{ 2, 4 },
         Edge{ 3, 4 },
};


Point2D projection2(Point3D point)
{
    return Point2D{ WindowSizeX / 2 + (point.x * FL) / (FL + point.z) * 100, WindowSizeY / 2 + (point.y * FL) / (FL + point.z) * 100 };
}

Point3D rotateOnX(Point3D point)
{
    Point3D rotatedPoint;
    rotatedPoint.x = point.x + 0 * point.y + 0 * point.z;
    rotatedPoint.y = 0 * point.x + cos(rotation) * point.y - sin(rotation) * point.z;
    rotatedPoint.z = 0 * point.x + sin(rotation) * point.y + cos(rotation) * point.z;
    return rotatedPoint;
}

Point3D rotateOnY(Point3D point)
{
    Point3D rotatedPoint;
    rotatedPoint.x = cos(rotation) * point.x + sin(rotation) * point.z;
    rotatedPoint.y = point.y;
    rotatedPoint.z = -sin(rotation) * point.x + cos(rotation) * point.z;
    return rotatedPoint;
}

Point3D scaleOut(Point3D point)
{
    Point3D scaledPoint;
    scaledPoint.x = point.x * scaledValue;
    scaledPoint.y = point.y * scaledValue;
    scaledPoint.z = point.z * scaledValue;
    return scaledPoint;
}

void render2(std::vector<Point3D> points, std::vector<Edge> edges)
{
    iterations++;
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    rotation = rotation + 1 * DeltaTime;

    for (auto& edge : edges) {
        Point3D rotatedPointStart = rotateOnY(points[edge.start]);
        Point3D rotatedPointEnd = rotateOnY(points[edge.end]);
        Point2D start = projection2(rotatedPointStart);
        Point2D end = projection2(rotatedPointEnd);

        SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
    }

    SDL_RenderPresent(renderer);
    auto time2 = std::chrono::high_resolution_clock::now();
    duration = time2 - time1;
    DeltaTime = duration.count();
    time1 = time2;
}

//Main functions are the start point of this C++ solution so the solution will not run if two main functions are
//in a single solution. To run this file, comment or delete (at your own peril) the main file in the cube.cpp file
//and remove the comments from the main file below

//default C++ function
int main(int argc, char* argv[])
{
    iterations = 0;
    //creates a centered window with 960 width and 540 height
    window = SDL_CreateWindow("Cube Renderer No OOP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_GetWindowSize(window, &WindowSizeX, &WindowSizeY);

    bool running = true;

    while (running)
    {
        //if the quit button is pressed, the loop breaks
        if (SDL_QuitRequested()) { running = false; break; }

        //Calls the function created in the Renderer3D class (render 3D is an instance of the class defined above)
        render2(points2, edges2);
    }

    return 0;
}
//This file encapsulates all the code we did in the other three files here. As I became aware the issue
//was C++ structure and OOP. The same thing done in the previous three files in put here. 
//This is however not the best method to use to code as once you begin to extend, your codebase becomes
//harder to work with. 