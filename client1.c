#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <unistd.h>
#include "tictactoe.h"
#define CLEAR() printf("\033[H\033[J") //to clear the linux term

void displayGame(gData *p, char player);

int main(void)
{
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
        if(p->playerTurn == player && p->nextMove[0] == '*'){
            displayGame(p, player);
            printf("Please make a move:\n?>");
            fgets(nextMove, 10, stdin);
            memcpy(p->nextMove, nextMove, 3);
            p->nextMove[2] = '\0';
        }else{
            displayGame(p, player);
        }
        usleep(50000);
    }

    munmap(&p, sizeof(gData));
    return 0;
}

void displayGame(gData *p, char player)
{
    CLEAR();

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
