/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 */

#include <string.h>
#include <stdlib.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "./logo.h"

int x = 25, y = 19;
int obstacleX = 77, obstacleY = 19;
float velocidadeX = 1, incY = 1;
float obstacleIncY = 1;
float obstacleIncX = -1;
float gravity = 1;

void printPlayer(int nextY);
void jump(int *ch, int y);
void printObstacle(int nextX, int nextY);
void groundInit(int y);
int collision(int x,int y,int obstacleX,int obstacleY);

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
    timerInit(50);

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


        screenSetColor(GREEN, DARKGRAY);
        screenGotoxy(MINX + 1, MINY + 2);
        printf("  Score: ");
        screenSetColor(RED, DARKGRAY);
        screenGotoxy(MINX + 1, MINY + 3);
        printf("  Kills: ");

        screenSetColor(CYAN, DARKGRAY);
        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1)
        {
            printPlayer(y);

            if (y > 19)
            {
                y = 19;
            }

            jump(&ch, y);

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
            if (newObstacleX < MINX + 2)
            {
                screenGotoxy(newObstacleX, obstacleY);
                printf("     ");
            }else{
                printObstacle(newObstacleX, obstacleY);
            }

            if (collision(x, y, obstacleX, obstacleY) == 1){
                break;
            }

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

void jump(int *ch, int y) {
    if (y < 19) {
        printPlayer(y + gravity);
    }
    if (*ch == 32 && y == 19) {
        int newPlayerY = y - 6;
        *ch = 0;
        printPlayer(newPlayerY + gravity);
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
                screenSetColor(LIGHTGREEN, DARKGRAY);
                printf("T");
            } else {
                screenSetColor(BLACK, DARKGRAY);
                if (i % 2 == 0) {
                    printf("/");
                } else {
                    printf("|");
                }
            }
        }
    }
}

int collision(int x,int y,int obstacleX,int obstacleY){
    if (x == obstacleX && y == obstacleY){
        return 1;
    }else{
        return 0;
    }
}