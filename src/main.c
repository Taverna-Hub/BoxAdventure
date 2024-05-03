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

int x = 34, y = 12;
int obstacleX = 30, obstacleY = 2;
float velocidadeX = 1, incY = 1;
float obstacleIncY = 1;

void printHello(int nextX, int nextY)
{
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(x, y);
    printf("           ");
    x = nextX;
    y = nextY;
    screenGotoxy(x, y);
    printf("Hello World");
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

void printGround(int y) {
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


    screenInit(1);
    timerInit(50);

    printHello(x, y);
    printObstacle(obstacleX, obstacleY);

    screenGotoxy(MINX, MAXY);

    screenUpdate();

    while (ch != 10) // enter
    {

        if (keyhit()) 
        {
            ch = readch();
            screenUpdate();
        }

        printGround(20);

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
            if (ch == 100)
            {
                int newHelloX = x + velocidadeX;
                if (newHelloX <= (MAXX - strlen("Hello World") - 1))
                {
                    velocidadeX = 1;
                }

                if (newHelloX >= (MAXX - strlen("Hello World") - 1))
                {
                    velocidadeX = 0;
                }
                ch = 0;

                printHello(newHelloX, y);
            }

            if (ch == 97)
            {
                int newHelloX = x - velocidadeX;

                if (!(newHelloX <= MINX + 1))
                {
                    velocidadeX = 1;
                }
                if (newHelloX <= MINX + 1)
                {
                    velocidadeX = 0;
                }

                printHello(newHelloX, y);
                ch = 0;
            }

            if (ch == 119)
            {

                int newY = y - incY;

                if (newY <= MAXY + 1)
                {
                    incY = 1;
                }
                if (newY >= MAXY + 1)
                {
                    incY = 0;
                }

                printHello(x, newY);
                ch = 0;
            }

            if (ch == 115)
            {

                int newY = y + incY;
                if (newY <= MINY - 1)
                {
                    incY = 0;
                }
                if (newY >= MINY - 1)
                {
                    incY = 1;
                }

                printHello(x, newY);
                ch = 0;
            }

            int newObstacleY = obstacleIncY + obstacleY;
            if (newObstacleY >= MAXY - 1 || newObstacleY <= MINY + 1)
            {
                obstacleIncY = -obstacleIncY;
            }

            printObstacle(obstacleX, newObstacleY);

            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}