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

struct element {
    int x;
    int y;
    int velX;
    int is_dead;
};

int bossX = 78;
int velY = 0;
int score = 0, kills = 0, scoreCounter = 0;
float gravity = 1;
float pastY;
int frame = 1;

void groundInit(int y);
void physics(int y, struct element * player);

void printBox(int nextX, int nextY, struct element * box, struct element * player);
void printPlayer(int nextY, struct element * player);
void printObstacle(int nextY, int nextX, struct element * obstacle);
void printBoss(int xis);
void printMessage(int kills);

int collisionElement(int x, int y, int obstacleX, int obstacleY);

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

    struct element * player = (struct element *)malloc(sizeof(struct element));
    player->x = 25;
    player->y = 19;
    player->velX = 1;
    
    struct element * obstacle[3];

    int initialObstacleX = 77;

    for (int i = 0; i < 3; i++) {
        obstacle[i] = (struct element *)malloc(sizeof(struct element));
        obstacle[i]->x = initialObstacleX;
        obstacle[i]->y = 19;
        obstacle[i]->velX = -1;
        initialObstacleX -= 10;
    }

    struct element * box[2];

    int initialBoxX = 70;

    for (int i = 0; i < 2; i++) {
        box[i] = (struct element *)malloc(sizeof(struct element));
        box[i]->x = initialBoxX;
        box[i]->y = 19;
        box[i]->velX = -1;
        obstacle[i]->is_dead = 0;
        initialBoxX -= 10;
    }

    printObstacle(obstacle[0]->x, obstacle[0]->y, obstacle[0]);


    screenGotoxy(MINX, MAXY);

    screenUpdate();

    while (ch != 10)
    {

        if (keyhit()) 
        {
            ch = readch();
            screenUpdate();
        }

        screenSetColor(GREEN, DARKGRAY);
        screenGotoxy(MINX + 1, MINY + 2);
        printf("  Score: ");

        screenSetColor(CYAN, DARKGRAY);

        if (timerTimeOver() == 1)
        {
            groundInit(20);

            screenGotoxy(MINX + 10, MINY + 2);
            printf("%d", score);
            if (kills > 0) {
                screenSetColor(RED, DARKGRAY);
                screenGotoxy(MINX + 1, MINY + 3);
                printf("  Kills: ");
                printMessage(kills);
                screenSetColor(CYAN, DARKGRAY);
                screenGotoxy(MINX + 10, MINX + 3);
                printf("%d", kills);
            }

            printPlayer(player->y, player);

            if (player->y > 19)
            {
                player->y = 19;
            }

            /* JUMP */
            if (ch == 32 && velY == 0) {
                velY = 3;
            }
            ch = 0;

            physics(player->y, player);

            int newObstacleX = obstacle[0]->x + obstacle[0]->velX;
            if (newObstacleX < MINX + 2)
            {   
                screenGotoxy(obstacle[0]->x, obstacle[0]->y);
                printf("     ");
                obstacle[0]->x = 77;
                screenGotoxy(obstacle[0]->x, obstacle[0]->y);
                printObstacle(obstacle[0]->x, obstacle[0]->y, obstacle[0]);
                // scoreCounter = 0;
            } else {
                printObstacle(newObstacleX, obstacle[0]->y, obstacle[0]);
            }

            if (collisionElement(player->x, player->y, obstacle[0]->x, obstacle[0]->y) == 1){
                screenGotoxy(MINX + 15, MINY + 2);
                break;
            }

            /* if (collisionElement(player->x, player->y, box[0]->x, box[0]->y) == 1 && player->x != pastY && box[0]->x == player->x){
                printPlayer(player->y, player);
            } */

            int newBoxX = box[0]->x + box[0]->velX;
            if (newBoxX < MINX + 2)
            {
                screenGotoxy(newBoxX, box[0]->y);
                printf("     ");
                box[0]->x = 77;
                screenGotoxy(box[0]->x, box[0]->y);
                printBox(box[0]->x, box[0]->y, box[0], player);
            }else{
                printBox(newBoxX, box[0]->y, box[0], player);
            }

            if (kills >= 20) {
                if (player->x <= 32) {
                    player->x = player->x + 1;
                    printPlayer(player->y, player);
                    frame++;
                }
                printMessage(kills);
                if (frame >= 7) {
                    printBoss(bossX);
                    bossX--;
                }

                for (int i = 0; i < 5; i++) {
                    screenGotoxy(25, 19-i);
                    printf(" ");
                }
            }

            pastY = player->y;
            screenUpdate();
        }
    } 

    free(player);

    for (int i = 0; i < 3; i++) {
        free(obstacle[i]);
    }

    for (int i = 0; i < 2; i++) {
        free(box[i]);
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}

void printPlayer(int nextY, struct element * player)
{
    screenGotoxy(player->x, player->y);
    printf(" ");

    player->y = nextY;

    screenGotoxy(player->x, player->y);
    printf("🔵");
}

void physics(int y, struct element * player) {
    if (velY > 0 || y < 19){
        y -= velY;
        velY -= gravity;
        printPlayer(y, player);
    }
    if (y > 19) {
        printPlayer(19, player);
    }
    if (y == 19 && velY < 0)
    {
        velY = 0;
    }
}

void printObstacle(int nextX, int nextY, struct element * obstacle)
{
    screenGotoxy(obstacle->x, obstacle->y);
    printf(" ");

    obstacle->x = nextX;
    obstacle->y = nextY;

    screenGotoxy(obstacle->x, obstacle->y);
    printf("🔥");
    /* if (obstacleX <= x){
        if (scoreCounter != 1){
            score++;
            scoreCounter += 1;
        }
    } */
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

int collisionElement(int x, int y, int obstacleX, int obstacleY){
    if (x == obstacleX && y == obstacleY){
        return 1;
    }else{
        return 0;
    }
}

void printBox(int nextX, int nextY, struct element * box, struct element * player){
    screenGotoxy(box->x, box->y-2);
    printf(" ");
    screenGotoxy(box->x, box->y);
    printf(" ");

    box->x = nextX;
    box->y = nextY;

    screenGotoxy(box->x, box->y);
    if (nextX <= player->x) {
        printf("🟥");
        if (box->is_dead == 0) {
            score++;
            kills++;
            scoreCounter += 1;
            box->is_dead = 1;
        }
    } else {
        screenGotoxy(box->x, box->y-2);
        screenSetColor(GREEN, DARKGRAY);
        printf("v");
        screenGotoxy(box->x, box->y);
        printf("⬜");
        screenSetColor(CYAN, DARKGRAY);
        box->is_dead = 0;

    }
    
}

void printBoss(int xis) {
    for (int j = 19; j > MINY+3; j--) {
        for (int i = xis; i < 79; i++) {
            if (i >= 50) {
                screenGotoxy(i, j);
                screenSetColor(WHITE, DARKGRAY);
                printf("#");
            }
        }
    }
}

void printMessage(int kills) {
    screenGotoxy(MINX + 18, MINY + 10);
    //AJEITAR COMO APAGA AS MENSAGENS
    printf("                                      ");
    if (kills < 10) {
        screenGotoxy(MINX+18, MINY+10);
        screenSetColor(RED, DARKGRAY);
        printf("O que você fez? Você matou ele");
        screenSetColor(CYAN, DARKGRAY);
    } else if (kills < 20) {
        screenGotoxy(MINX+18, MINY+10);
        screenSetColor(RED, DARKGRAY);
        printf("Para com isso, seu assassino");
        screenSetColor(CYAN, DARKGRAY);
    } else if (kills < 50) {
        screenGotoxy(MINX+18, MINY+10);
        screenSetColor(RED, DARKGRAY);
        printf("É hora de pagar pelos seus pecados...");
        screenSetColor(CYAN, DARKGRAY);
    }
}