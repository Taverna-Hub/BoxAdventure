/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "./logo.h"

int x = 25, y = 19;
int yChange = 0;
int obstacleX = 77, obstacleY = 19;
float velocidadeX = 1, incY = 1;
float obstacleIncY = 1;
float obstacleIncX = -1;
float gravity = 1;
float blockIncX = -1;
float blockX = 60, blockY = 19;
float pastY;

void printPlayer(int nextY);
void physics(int y);
void printObstacle(int nextX, int nextY);
void groundInit(int y);
int collisionObstacle(int x,int y,int obstacleX,int obstacleY);
void printBlock(int nextX, int blockX);

int main()
{
    static int ch = 0;
    keyboardInit();
    system("clear");
    printf("%s\n", logo);

    while (ch != 32) {
        if (keyhit())
        {
            ch = readch();
        }
    }
    ch = 0;


    screenInit(1);
    timerInit(40);

    printObstacle(obstacleX, obstacleY);
    groundInit(20);

    screenGotoxy(MINX, MAXY);

    screenUpdate();

    while (ch != 10) // enter
    {

        if (keyhit()) 
        {
            ch = readch();
            screenUpdate();
        }


        screenSetColor(GREEN, WHITE);
        screenGotoxy(MINX + 1, MINY + 2);
        printf("  Score: ");
        screenSetColor(RED, WHITE);
        screenGotoxy(MINX + 1, MINY + 3);
        printf("  Kills: ");

        screenSetColor(CYAN, WHITE);
        // Update game state (move elements, verify collisionObstacle, etc)
        if (timerTimeOver() == 1)
        {
            printPlayer(y);

            if (y > 19)
            {
                y = 19;
            }

            if (ch == 32 && yChange == 0) {
                yChange = 3;
            }
            ch = 0;
            physics(y);

            // if (ch == 100)
            // {
            //     int newHelloX = x + velocidadeX;
            //     if (newHelloX <= (MAXX - strlen("Hello World") - 1))
            //     {
            //         velocidadeX = 1;
            //     }

            //     if (newHelloX >= (MAXX - strlen("Hello World") - 1))
            //     {
            //         velocidadeX = 0;
            //     }
            //     ch = 0;

            //     printHello(newHelloX, y);
            // }

            // if (ch == 97)
            // {
            //     int newHelloX = x - velocidadeX;

            //     if (!(newHelloX <= MINX + 1))
            //     {
            //         velocidadeX = 1;
            //     }
            //     if (newHelloX <= MINX + 1)
            //     {
            //         velocidadeX = 0;
            //     }

            //     printHello(newHelloX, y);
            //     ch = 0;
            // }

            // if (ch == 119)
            // {

            //     int newY = y - incY;

            //     if (newY <= MAXY + 1)
            //     {
            //         incY = 1;
            //     }
            //     if (newY >= MAXY + 1)
            //     {
            //         incY = 0;
            //     }

            //     printHello(x, newY);
            //     ch = 0;
            // }

            // if (ch == 115)
            // {

            //     int newY = y + incY;
            //     if (newY <= MINY - 1)
            //     {
            //         incY = 0;
            //     }
            //     if (newY >= MINY - 1)
            //     {
            //         incY = 1;
            //     }

            //     printHello(x, newY);
            //     ch = 0;
            // }

            int newObstacleX = obstacleIncX + obstacleX;
            if (newObstacleX < MINX + 1)
            {
                obstacleX = 77;
                obstacleY = 19;
                screenGotoxy(obstacleX, obstacleY);
                printObstacle(obstacleX, obstacleY);
            }else{
                printObstacle(newObstacleX, obstacleY);
            }

            if (collisionObstacle(x, y, obstacleX, obstacleY) == 1){
                break;
            }

            if (collisionObstacle(x, y, blockX, blockY) == 1 && y != pastY && x == blockX){
                y = blockY - 1;
                printPlayer(y);
            } else if (collisionObstacle(x, y, blockX, blockY) == 1){
                break;
            }


            int newBlockX = blockIncX + blockX;
            if (newBlockX < MINX + 1)
            {
                blockX = 77;
                blockY = 19;
                screenGotoxy(blockX, blockY);
                printBlock(blockX, blockY);
            }else{
                printBlock(newBlockX, blockY);
            }

            pastY = y;
            screenUpdate();
        }
    } 

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}

void printPlayer(int nextY)
{
    screenGotoxy(x, y);
    printf(" ");

    y = nextY;
    screenGotoxy(x, y);
    printf("🔵");
}

void physics(int y) {
    if (yChange > 0 || y < 19){
        y -= yChange;
        yChange -= gravity;
        printPlayer(y);
    }
    if (y > 19) {
        printPlayer(19);
    }
    if (y == 19 && yChange < 0)
    {
        yChange = 0;
    }
}

void printObstacle(int nextX, int nextY)
{
    screenGotoxy(obstacleX, obstacleY);
    printf(" ");
    obstacleX = nextX;
    obstacleY = nextY;
    screenGotoxy(obstacleX, obstacleY);
    printf("🔥");
}

void groundInit(int y) {
    for (int j = y; j < MAXY; j++) {
        for (int i = 2; i < 79; i++) {
            screenGotoxy(i, j);
            if (j == y) {
                screenSetColor(LIGHTGREEN, WHITE);
                printf("T");
            } else {
                screenSetColor(BLACK, WHITE);
                if (i % 2 == 0) {
                    printf("/");
                } else {
                    printf("|");
                }
            }
        }
    }
}

int collisionObstacle(int x,int y,int obstacleX,int obstacleY){
    if (x == obstacleX && y == obstacleY){
        return 1;
    }else{
        return 0;
    }
}

void printBlock(int nextX, int nextY){
    screenGotoxy(blockX, blockY);
    printf(" ");
    blockX = nextX;
    blockY = nextY;
    screenGotoxy(blockX, blockY);
    printf("⬜");
}