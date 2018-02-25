#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <unistd.h>
#include "tictactoe.h"
#include <stdlib.h>
#include <time.h>
#define CLEAR() printf("\033[H\033[J") //to clear the linux term

void selectRandomMove(char *game, char *nextMove);

void displayGame(gData *p, char player);

int main(void)
{
    srand(time(NULL));
    char name[] = "/gameState";
    int smfd = shm_open (name, O_RDWR, 0660); 

    if(smfd == -1){
        perror("Unable to open shared memory\n");
        return 1;
    }

    gData *p;
    p = mmap(NULL, sizeof(gData), PROT_WRITE, MAP_SHARED, smfd, 0);

    if(p < 0){
        perror("mmap failed.\n");
        return 1;
    }

    printf("p->game read from master process = %s\n", p->game);

    char player;
    printf("Trying to be player X..\n");
    if(p->playerX == '*'){
        p->playerX = 'X';
        player = 'X';
    }else if(p->playerO == '*'){
        p->playerO = 'O';
        player = 'O';
    }else{
        printf("No available player slots\n");
        return 1;
    }
    printf("This instance is player %c\n", player);

    /* Main loop */
    while(1){
        char nextMove[10];
        //Turn?
        if(p->playerTurn == player && p->lck){
            displayGame(p, player);
            
            selectRandomMove(p->game, nextMove);

            p->lck = 0; //lock mem
            memcpy(p->nextMove, nextMove, 3);
            p->nextMove[2] = '\0';
        }else{
            displayGame(p, player);
        }

        usleep(UDELAY);
    }

    munmap(&p, sizeof(gData));
    return 0;
}

void displayGame(gData *p, char player)
{
    CLEAR();
    printf("Player: %c. Scores; X: %d, O: %d, ties: %d\n\n", \
            player, p->scoreX, p->scoreO, p->ties);

    int i;
    int j = 9;
    
    for(i = 3; i > 0; i--){
        printf("%i    %c | %c | %c \n", i, p->game[j - 3], p->game[j - 2], p->game[j - 1]);
        if(i > 1)
            printf("    ---|---|---\n");
        j -= 3;
    }
    printf("\n     a   b   c\n\n");
    printf("%s\n", p->bcastMsg);
}

void selectRandomMove(char *game, char *nextMove)
{
    int rnd = rand() % 9;
    while(game[rnd] != '*')
        rnd = rand() % 9;

    char rndMv[3];

    if(rnd == 0)
        memcpy(rndMv, "a1\0", 3);
    else if(rnd == 1)
        memcpy(rndMv, "b1\0", 3);
    else if(rnd == 2)
        memcpy(rndMv, "c1\0", 3);
    else if(rnd == 3)
        memcpy(rndMv, "a2\0", 3);
    else if(rnd == 4)
        memcpy(rndMv, "b2\0", 3);
    else if(rnd == 5)
        memcpy(rndMv, "c2\0", 3);
    else if(rnd == 6)
        memcpy(rndMv, "a3\0", 3);
    else if(rnd == 7)
        memcpy(rndMv, "b3\0", 3);
    else if(rnd == 8)
        memcpy(rndMv, "c3\0", 3);

    strncpy(nextMove, rndMv, 3);
}

void selectBestMove(char *game)
{
    //Possible moves.. are *
        
}













