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
#include "./ascii.h"

#define KILLS_LIMIT 30

struct element {
    int x;
    int y;
    float velX;
    int colision;
    int is_dead;
    int lives;
    int side;
    int lifePiece;
};

struct score {
    char name[21];
    int points;
    struct score *next;
};

/* void printLinkedList(struct score* head) {
    struct score* temp = head;
    printf("Lista de pontuações:\n");
    while (temp != NULL) {
        printf("Nome: %s, Pontuação: %d\n", temp->name, temp->points);
        temp = temp->next;
    }
} */

int bossX = 78;
int velY = 0;
int score = 0, kills = 0, scoreCounter = 0;
float gravity = 1;
int frame = 1;
int frameBox = 1;
int frameBlink = 10;
int lastRound = 0;
int phase = 1;
int black = 0;
int killed = 0;

void addScore(struct score *head, char * name, int points);
void orderAddList(struct score **head, char * name, int points);
void saveScore(struct score *head);
void printLeaderboard(struct score *head);

void groundInit(int y, int stop);
void physics(int y, struct element * player);

void printBox(int nextX, int nextY, struct element * box, struct element * player);
void printPlayer(int nextY, struct element * player);
void printObstacle(int nextY, int nextX, struct element * obstacle);
void printBoss(int xis);
void printMessage(int kills, int black);
void printLives(int lives);
void printLifePiece(int lifePiece);
void obstacleSpawn(int phase, struct element *obstacle);
void boxSpawn(int phase, struct element *box, struct element *player);

int collisionElement(int x, int y, int obstacleX, int obstacleY);
int collisionBox(int x, int y, int obstacleX, int obstacleY);

struct element * createElement();

struct element* createElement() {
    struct element* newElement = (struct element *)malloc(sizeof(struct element));

    if (newElement != NULL) { // Verificar se a alocação de memória foi bem-sucedida
        newElement->x = 0; // Valor padrão
        newElement->y = 0; // Valor padrão
        newElement->velX = 0; // Valor padrão
        newElement->colision = 2; // Definindo o valor padrão de colision como 2
        newElement->is_dead = 0; // Valor padrão
        newElement->lives = 0; // Valor padrão
        newElement->lifePiece = 0; // Valor padrão
    }

    return newElement; // Retornar o ponteiro para a estrutura
}

int main()
{   
    static int ch = 0;

    screenInit(0);
    
    screenHideCursor();

    keyboardInit();

    system("clear");
    screenSetColor(CYAN, DARKGRAY);
    printf("%s\n", logo);
    screenSetColor(YELLOW, DARKGRAY);
    printf("%s\n", start);

    while (ch != 32) {
        if (keyhit())
        {
            ch = readch();
        }
    }
    ch = 0;

    screenSetColor(CYAN, DARKGRAY);
    screenInit(1);

    timerInit(40);

    struct element * player = createElement();
    player->x = 25;
    player->y = 19;
    player->velX = 1;
    player->colision = 1;
    player->lives = 1;
    player->lifePiece = 0;
    
    struct element * obstacle[3];

    int initialObstacleX = 77;

    for (int i = 0; i < 3; i++) {
        obstacle[i] = createElement();
        obstacle[i]->x = initialObstacleX;
        obstacle[i]->y = 19;
        obstacle[i]->velX = -1;
        initialObstacleX -= 10;
    }

    struct element * box[2];

    int initialBoxX = 55;

    for (int i = 0; i < 2; i++) {
        box[i] = (struct element *)malloc(sizeof(struct element));
        box[i]->x = initialBoxX;
        box[i]->y = 16;
        box[i]->velX = -1;
        box[i]->side = 0;
        obstacle[i]->is_dead = 0;
        initialBoxX -= 10;
    }

    printObstacle(obstacle[0]->x, obstacle[0]->y, obstacle[0]);

    screenGotoxy(MINX, MAXY);

    screenUpdate();

    while (ch != 10)
    {
        if (score >= 10000) {
            if (phase > 1) {
                box[0]->velX = 1.05;
                obstacle[0]->velX = 2;
                obstacle[1]->velX = 1;
                obstacle[2]->velX = 1;
            } else {
                box[0]->velX = -2;
                obstacle[0]->velX = -2;
                obstacle[1]->velX = -2;
                obstacle[2]->velX = -1;
            }
        } else if (score >= 5000) {
            if (phase > 1) {
                box[0]->velX = 1.05;
                obstacle[0]->velX = 1.05;
                obstacle[1]->velX = 1.05;
                obstacle[2]->velX = 1;
            } else {
                box[0]->velX = -1.05;
                obstacle[0]->velX = -1.05;
                obstacle[1]->velX = -1.05;
                obstacle[2]->velX = -1;
            }
        } 

        if (keyhit()) 
        {
            ch = readch();
            screenUpdate();
        }

        screenSetColor(CYAN, DARKGRAY);

        if (timerTimeOver() == 1)
        { 
            groundInit(20, 0);

            if (player->lives >= 1) {
                screenSetColor(GREEN, DARKGRAY);
                screenGotoxy(MINX + 1, MINY + 2);
                printf("  Score: ");
                screenSetColor(CYAN, DARKGRAY);
                screenGotoxy(MINX + 10, MINY + 2);
                printf("%d", score);
                if (kills > 0) {
                    screenSetColor(RED, DARKGRAY);
                    screenGotoxy(MINX + 1, MINY + 3);
                    printf("  Kills: ");
                    printMessage(kills, black);
                    screenSetColor(CYAN, DARKGRAY);
                    screenGotoxy(MINX + 10, MINY + 3);
                    printf("%d", kills);
                }
            } else if (player->lives < 1){
                screenGotoxy(MINX + 1, MINY + 2);
                printf("                                                                               ");
                screenGotoxy(MINX + 10, MINY + 2);
                printf("                                                                               ");
                screenGotoxy(MINX + 1, MINY + 3);
                printf("                                                                               ");
                screenGotoxy(MINX + 10, MINY + 3);
                printf("                                                                               ");
            }

            if (kills >= KILLS_LIMIT) {
                if (black == 0) {
                    if (frameBlink <= 10) {
                        screenSetColor(RED, DARKGRAY);
                        screenInit(1);
                        frameBlink++;
                    }
                    black = 1;
                } else {
                    if (frameBlink <= 10) {
                        screenSetColor(RED, DARKGRAY);
                        screenInit(1);
                    }
                    black = 0;
                }
            }

            printLives(player->lives);
            printLifePiece(player->lifePiece);
            
            printPlayer(player->y, player);

            if (player->y > 19)
            {
                player->y = 19;
            }

            /* JUMP */
            if (ch == 32 && velY == 0 && player->y == 19) {
                velY = 3;
            }
            ch = 0;

            physics(player->y, player);

            // Spawn e movimentação de obstáculos e caixas
            obstacleSpawn(phase, obstacle[0]);
            if (score >= 2000) {
                obstacleSpawn(phase, obstacle[1]);
            }
            if (score >= 10000) {
                obstacleSpawn(phase, obstacle[2]);
            }
            boxSpawn(phase, box[0], player);


            if (score >= 2000) {
                if (collisionElement(player->x, player->y, obstacle[1]->x, obstacle[1]->y)){
                if (player->lives <= 1 ) {
                    
                    groundInit(20, 1);

                    screenGotoxy(MINX + 1, MINY + 2);
                    printf("                                                                               ");
                    screenGotoxy(MINX + 10, MINY + 2);
                    printf("                                                                               ");
                    screenGotoxy(MINX + 1, MINY + 3);
                    printf("                                                                               ");
                    screenGotoxy(MINX + 10, MINY + 3);
                    printf("                                                                               ");
                
                    screenInit(0);
                    screenSetColor(RED, DARKGRAY);
                    printf("%s\n", gameOver);

                    screenSetColor(YELLOW, DARKGRAY);
                    printf("%s\n", end);

                    while (ch != 10 && ch != 32) {
                        if (keyhit())
                        {
                            ch = readch();
                        }
                    }
                    break;
                } else {
                    player->lives--;
                }
            }
            }
            if (score >= 10000) {
                if (collisionElement(player->x, player->y, obstacle[2]->x, obstacle[2]->y)){
                if (player->lives <= 1 ) {
                    
                    groundInit(20, 1);

                    screenGotoxy(MINX + 1, MINY + 2);
                    printf("                                                                               ");
                    screenGotoxy(MINX + 10, MINY + 2);
                    printf("                                                                               ");
                    screenGotoxy(MINX + 1, MINY + 3);
                    printf("                                                                               ");
                    screenGotoxy(MINX + 10, MINY + 3);
                    printf("                                                                               ");
                
                    screenInit(0);
                    screenSetColor(RED, DARKGRAY);
                    printf("%s\n", gameOver);

                    screenSetColor(YELLOW, DARKGRAY);
                    printf("%s\n", end);

                    while (ch != 10 && ch != 32) {
                        if (keyhit())
                        {
                            ch = readch();
                        }
                    }
                    break;
                } else {
                    player->lives--;
                }
            }
            }

            // Player perde vida ou acaba o jogo se perder todas vidas
            if (collisionElement(player->x, player->y, obstacle[0]->x, obstacle[0]->y)){
                if (player->lives <= 1 ) {
                    
                    groundInit(20, 1);

                    screenGotoxy(MINX + 1, MINY + 2);
                    printf("                                                                               ");
                    screenGotoxy(MINX + 10, MINY + 2);
                    printf("                                                                               ");
                    screenGotoxy(MINX + 1, MINY + 3);
                    printf("                                                                               ");
                    screenGotoxy(MINX + 10, MINY + 3);
                    printf("                                                                               ");
                
                    screenInit(0);
                    screenSetColor(RED, DARKGRAY);
                    printf("%s\n", gameOver);

                    screenSetColor(YELLOW, DARKGRAY);
                    printf("%s\n\n\n", end);

                    screenSetColor(YELLOW, DARKGRAY);
                    printf("%s\n", endScore);

                    while (ch != 10 && ch != 32) {
                        if (keyhit())
                        {
                            ch = readch();
                        }
                    }
                    break;
                } else {
                    player->lives--;
                }
            }

            if (player->x == obstacle[0]->x && player->y != obstacle[0]->y){
                score+=100;
            }

            if (score >= 2000) {
                if (player->x == obstacle[1]->x && player->y != obstacle[1]->y){
                    score+=100;
                }
            }
            if (score >= 10000) {
                if (player->x == obstacle[2]->x && player->y != obstacle[2]->y){
                    score+=100;
                }
            }

            if (collisionElement(player->x, player->y, box[0]->x, box[0]->y) == 1){
                if (box[0]->is_dead == 1){
                    killed = 1;
                    player->lifePiece++;
                }
            }

            if (player->lifePiece >= 5){
                player->lives++;
                player->lifePiece = 0;
            }

            if (kills >= KILLS_LIMIT) {
                if (player->x <= 32) {
                    player->x = player->x + 1;
                    printPlayer(player->y, player);
                    frame++;
                    phase++;
                }

                printMessage(kills, black);
                if (frame >= 7) {
                    printBoss(bossX);
                    bossX--;
                }

            }

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

    screenInit(0);

    screenSetColor(YELLOW, DARKGRAY);
    printf("%s\n", leaderboard);

    struct score * head = (struct score *)malloc(sizeof(struct score));
    strcpy(head->name, "head");
    head->points = 0;
    head->next = NULL;

    FILE *open;

    open = fopen("./scores.txt", "r");

    if (open != NULL) {
        while (!feof(open)) {
            int points;
            char name[21];
            fscanf(open,"%s %d", name, &points);
            if (strcmp(name, "") != 0) {
                orderAddList(&head, name, points);
            }
        }

    fclose(open);
    printLeaderboard(head);

    /* printLinkedList(head); */

    if (ch == 32) {
        char nameTemp[21];
        screenShowCursor();
        screenSetColor(CYAN, DARKGRAY);
        screenGotoxy(MINX + 23, MINY + 17);
        printf("Your score: ");
        screenSetColor(YELLOW, DARKGRAY);
        printf("%d", score);
        screenSetColor(CYAN, DARKGRAY); 
        screenGotoxy(MINX + 23, MINY + 18);
        printf("Enter your name: ");
        screenSetColor(WHITE, DARKGRAY); 
        scanf("%21s", nameTemp); //21s serve de limite de caracters

        orderAddList(&head, nameTemp, score);

        saveScore(head);
    }
    } else {
        printf("deu ruim");
    }
    screenDestroy();

    free(head);

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
}

void groundInit(int y, int stop) {
    if (stop == 0) {
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
    } else {
        for (int j = y; j < MAXY; j++) {
            for (int i = 2; i < 79; i++) {
                screenGotoxy(i, j);
                printf(" ");
            }
        }
    }
}

int collisionElement(int x, int y, int obstacleX, int obstacleY){
    if (kills < KILLS_LIMIT) {
        if (x == obstacleX && y == obstacleY ){
            return 1;
        }else{
            return 0;
        }
    } else {
        if (32 == obstacleX && y == obstacleY ){
            return 1;
        }else{
            return 0;
        }
    }
}

int collisionBox(int x, int y, int boxX, int boxY){
    if ((x == boxX || x == boxX+1 || x == boxX-1 || x == boxX+2 || x == boxX-2) && (y == boxY || y == boxY+1 || y == boxY-1 || y == boxY+2 || y == boxY-2) ){
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
    if (nextX <= player->x && collisionBox(player->x, player->y, box->x, box->y-2) == 1) {
        printf("🟥");
        if (box->is_dead == 0) {
            score += 200;
            kills++;
            if (player->lives < 3) {
                player->lifePiece++;
            }  
            scoreCounter += 1;
            box->is_dead = 1;
        }
    } else{
        if (box->is_dead != 1 && kills < 1) {
            screenGotoxy(box->x, box->y-2);
            screenSetColor(GREEN, DARKGRAY);
            printf("v");
        }

        screenGotoxy(box->x, box->y);
        printf("%s", (box->is_dead != 1) ? "⬜" : "🟥");
        screenSetColor(CYAN, DARKGRAY);
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

void printMessage(int kills, int black) {
    screenGotoxy(MINX + 18, MINY + 10);
    //AJEITAR COMO APAGA AS MENSAGENS
    if (kills <= KILLS_LIMIT) {
        printf("                                       ");
    } else {
        printf("                              ");
    }
    if (kills >= 1 && kills <= 3) {
        screenGotoxy(MINX+18, MINY+10);
        screenSetColor(RED, DARKGRAY);
        printf("What have you done? You just killed him");
        screenSetColor(CYAN, DARKGRAY);
    } else if (kills == 15 && kills <= 18) {
        screenGotoxy(MINX+24, MINY+10);
        screenSetColor(RED, DARKGRAY);
        printf("Stop that, you murderer");
        screenSetColor(CYAN, DARKGRAY);
    } else if (kills >= 25 && kills <= 27) {
        screenGotoxy(MINX+30, MINY+10);
        screenSetColor(RED, DARKGRAY);
        printf("I warned you");
        screenSetColor(CYAN, DARKGRAY);
    } else if (kills >= KILLS_LIMIT) {
        screenGotoxy(MINX+11, MINY+10);
        if (black == 1) {
            screenSetColor(BLACK, DARKGRAY);
        } else {
            screenSetColor(RED, DARKGRAY);
        }
        printf("It's time to pay for your sins");
        screenSetColor(CYAN, DARKGRAY);
    }
}

void printLives(int lives) {
    screenGotoxy(MINX + 1, MINY + 5);
    printf("  Lives: ");
    screenGotoxy(MINX + 10, MINY + 5);

    for (int i = 1;i < 4; i++){
        printf("%s", (i <=lives) ? "🔵" : "⚫");
    }
}

void printLifePiece(int lifePiece){
    screenGotoxy(MINX + 1, MINY + 6);
    screenSetColor(LIGHTMAGENTA, DARKGRAY);
    printf("  Life Pieces: ");
    screenGotoxy(MINX + 16, MINY + 6);
    printf("%d / 5", lifePiece);
}

void boxSpawn (int phase, struct element *box, struct element *player){
    if (phase == 1){

        int newBoxX = box->x + box->velX;

        if (score >= 10000) {
            if (box->side == 1) {
                    if (frameBox == 0) {
                        if (newBoxX - 3 > MINX+1) {
                            newBoxX = newBoxX - 4;
                        }
                    }
            } else {
                if (frameBox == 0) {
                    if (newBoxX + 3 < MAXX-2) {
                        newBoxX = newBoxX + 4;
                    }
                }
            }
        }

        if (newBoxX < MINX + 3)
        {
            screenGotoxy(newBoxX, box->y);
            printf("     ");
            screenGotoxy(newBoxX, box->y-2);
            printf("     ");
            box->x = 77;
            box->is_dead = 0;
            screenGotoxy(box->x, box->y);   
            printBox(box->x, box->y, box, player);
        } else {
            printBox(newBoxX, box->y, box, player);
            if (box->side == 0) {
                if (frameBox == 8) {
                    box->side = 1;
                    frameBox = 0;
                } else {
                    frameBox++;
                }
            } else {
                if (frameBox == 8) {
                    box->side = 0;
                    frameBox = 0;
                } else {
                    frameBox++;
                }
            }
        }
    } else {
        int newBoxX = box->x + box->velX;

        if (score >= 10000) {
            if (box->side == 1) {
                    if (frameBox == 0) {
                        if (newBoxX + 3 > MINX-1) {
                            newBoxX = newBoxX + 4;
                        }
                    }
            } else {
                if (frameBox == 0) {
                    if (newBoxX - 3 < MAXX+2) {
                        newBoxX = newBoxX - 4;
                    }
                }
            }
        }

        if (newBoxX > 50)
        {
            screenGotoxy(box->x, box->y);
            printf("      ");
            screenGotoxy(box->x, box->y-2);
            printf("      ");
            box->x = 2;
            box->is_dead = 0;
            screenGotoxy(box->x, box->y);
            printBox(box->x, box->y, box, player);
        } else {
            printBox(newBoxX, box->y, box, player);
            if (box->side == 0) {
                if (frameBox == 8) {
                    box->side = 1;
                    frameBox = 0;
                } else {
                    frameBox++;
                }
            } else {
                if (frameBox == 8) {
                    box->side = 0;
                    frameBox = 0;
                } else {
                    frameBox++;
                }
            }

        }
    }
}

void obstacleSpawn(int phase, struct element *obstacle){
    if (phase == 1){
        int newObstacleX = obstacle->x + obstacle->velX;
        if (newObstacleX < MINX + 3)
        {   
            screenGotoxy(obstacle->x, obstacle->y);
            printf("     ");
            obstacle->x = 77;
            screenGotoxy(obstacle->x, obstacle->y);
            printObstacle(obstacle->x, obstacle->y, obstacle);
        } else {
            printObstacle(newObstacleX, obstacle->y, obstacle);
        }
    } else {
        int newObstacleX = obstacle->x + obstacle->velX;
        if (newObstacleX > 50)
        {   
            screenGotoxy(obstacle->x, obstacle->y);
            printf("     ");
            obstacle->x = 2;
            screenGotoxy(obstacle->x, obstacle->y);
            printObstacle(obstacle->x, obstacle->y, obstacle);
        } else {
            printObstacle(newObstacleX, obstacle->y, obstacle);
        }
    }
}

void orderAddList(struct score **head, char * name, int points) {
    struct score * new;
    new = (struct score *)malloc(sizeof(struct score));
    strcpy(new->name, name);
    new->points = points;
    new->next = NULL;

    if (*head == NULL) {
        *head = new;
    } else if ((*head)->next == NULL) {
        if ((*head)->points < points) {
            new->next = *head;
            *head = new;
        } else {
            (*head)->next = new;
        }
    } else {
        struct score * temp = *head;
        if ((*head)->points < points) {
            new->next = *head;
            *head = new;
        } else {
            while (temp->next != NULL && temp->next->points > points) {
                temp = temp->next;
            }
            new->next = temp->next;
            temp->next = new;
        }
    }
}

void printLeaderboard(struct score * head) {
    struct score * temp = head;
    int added = 0;
    for (int i = 0; i < 5; i++) {
        screenSetColor(CYAN, DARKGRAY);
        screenGotoxy(49, 12+i);
        if (temp->points > 0) {
            if (i == 0) {
                printf("🏆 %s - ", temp->name);
                screenSetColor(YELLOW, DARKGRAY);
                printf("%d pts\n", temp->points);
                added += 1;
            } else if (i == 1) {
                printf("🥈 %s - ", temp->name);
                screenSetColor(YELLOW, DARKGRAY);
                printf("%d pts\n", temp->points);
            } else if (i == 2) {
                printf("🥉 %s - ", temp->name);
                screenSetColor(YELLOW, DARKGRAY);
                printf("%d pts\n", temp->points);
            } else {
                printf("🏅 %s - ", temp->name);
                screenSetColor(YELLOW, DARKGRAY);
                printf("%d pts\n", temp->points);
            }
        }
        temp = temp->next;
        if (temp == NULL) {
            break;
        }
    }
    if (added == 0) {
        screenSetColor(CYAN, DARKGRAY);
        printf("No records recorded\n");
    }
}

void saveScore(struct score *head) {
    struct score * temp = head;
    FILE *file = fopen("./scores.txt", "w");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo de scores!\n");
        return;
    }

    while (temp != NULL) {
        if (temp->points > 0) {
            fprintf(file, "%s %d\n", temp->name, temp->points);
        }
        temp = temp->next;
    }

    fclose(file);
}