#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <unistd.h>
#include <time.h>
#include "tictactoe.h"

gData *initGame(char *shmName);
int parseMove(gData *p);
int newGame(gData *p);
int findWinner(char *gameState);
int setMove(gData *p, char col, int row);

/* Some globals for the tps counter */
int t1;
int t2;
int tpsCnt = 0;
int tpsCntSum = 0;

int main(void)
{
    char shmName[] = "/gameState";

    gData *p = initGame(shmName);

    /* Parse client moves */
    while(1){
        if(p->nextMove[0] != '*'){
            
            parseMove(p);

            memcpy(p->nextMove, "* \0", 3);
        }

        usleep(UDELAY);
    }

    munmap(&p, sizeof(gData));

    shm_unlink(shmName);

    return 0;
}

/* Returns pointer to shared memory game state array */
gData *initGame(char *shmName)
{
    int smfd = shm_open (shmName, O_RDWR | O_CREAT, 0660); 

    if(smfd == -1){
        perror("Unable to open shared memory\n");
        shm_unlink(shmName);
        return NULL;
    }

    gData *p;
    p = mmap(NULL, sizeof(gData), PROT_WRITE, MAP_SHARED, smfd, 0);

    if (ftruncate (smfd, sizeof (gData)) == -1) 
        perror ("ftruncate");

    if(p < 0){
        perror("mmap failed.\n");
        munmap(&p, sizeof(gData));
        return NULL;
    }

    /* Initialize players to none */
    p->playerX = '*';
    p->playerO = '*';

    p->scoreX = 0;
    p->scoreO = 0;
    p->ties = 0;
    memcpy(p->nextMove, "* \0", 3);

    /* Init new gameState */
    int t1 = time(NULL);
    newGame(p);

    return p;
}

int parseMove(gData *p)
{
    char col = p->nextMove[0];
    int row = p->nextMove[1] - '0';

    if(setMove(p, col, row) == 0){
        printf("Setmove returns 0, move not allowed, move not allow retry\n");
    
        return 0;
    }
    
    /* Calculate draw */
    int i;
    int e = 0;

    for(i = 0; i < 8; i++){
        if(p->game[i] == '*')
            ++e;
    }
    
    /* Find tie */
    if(e == 0){
        memcpy(p->bcastMsg, "Draw. A new game begins soon..\0", 36);
        p->ties++;
        usleep(UDELAY2);
        tpsCnt += newGame(p);
        return 0;
    }

    /* Find a winner */
    if(findWinner(p->game) == 1){
        memcpy(p->bcastMsg, "X wins this game! Starting new game..\0", 38);
        p->winner = 'X';
        p->scoreX++;
        usleep(UDELAY2);
        tpsCnt += newGame(p);
        return 0;
    }else if(findWinner(p->game) == 2){
        memcpy(p->bcastMsg, "O wins this game! Starting new game..\0", 38);
        p->winner = 'O';
        p->scoreO++;
        usleep(UDELAY2);
        tpsCnt += newGame(p);
        return 0;
    }

    /* Switch turns */
    if(p->playerTurn == 'X')
        p->playerTurn = 'O';
    else
        p->playerTurn = 'X';
    /* Reset nextMove */
    memcpy(p->nextMove, "* \0", 3);

    return 0;
}

int newGame(gData *p)
{
    /* Calculate tps */
    t2 = time(NULL);
    if((t2 - t1) >= 1){
        t1 = time(NULL);
        tpsCntSum = tpsCnt;
        tpsCnt = 0;
    }
        
    /* Print scores and other stats */
    long int gamesPlayed = p->scoreX + p->scoreO + p->ties;
    printf("%s\tScores; X: %d, O: %d, ties: %d\t Game cnt: %li\ttpsCnt: %d\n", \
            p->game, p->scoreX, p->scoreO, p->ties, gamesPlayed, tpsCntSum);

    /* Init new gameState */
    memcpy(p->game, "*********\0", 10);

    p->playerTurn = 'X';

    memcpy(p->nextMove, "* \0", 3);
    memcpy(p->bcastMsg, "Welcome to a new game\0", 22);
    p->winner = '*';

    return 1; 
}

/* Returns int:
 * 0: no winner (yet)
 * 1: X wins
 * 2: O wins */
int findWinner(char *gameState)
{
    if(gameState[0] == 'X' && gameState[1] == 'X' && gameState[2] == 'X')
        return 1;
    else if(gameState[0] == 'O' && gameState[1] == 'O' && gameState[2] == 'O')
        return 2;
    else if(gameState[3] == 'X' && gameState[4] == 'X' && gameState[5] == 'X')
        return 1;
    else if(gameState[3] == 'O' && gameState[4] == 'O' && gameState[5] == 'O')
        return 2;
    else if(gameState[6] == 'X' && gameState[7] == 'X' && gameState[8] == 'X')
        return 1;
    else if(gameState[6] == 'O' && gameState[7] == 'O' && gameState[8] == 'O')
        return 2;
    else if(gameState[0] == 'X' && gameState[3] == 'X' && gameState[6] == 'X')
        return 1;
    else if(gameState[0] == 'O' && gameState[3] == 'O' && gameState[6] == 'O')
        return 2;
    else if(gameState[1] == 'X' && gameState[4] == 'X' && gameState[7] == 'X')
        return 1;
    else if(gameState[1] == 'O' && gameState[4] == 'O' && gameState[7] == 'O')
        return 2;
    else if(gameState[2] == 'X' && gameState[5] == 'X' && gameState[8] == 'X')
        return 1;
    else if(gameState[2] == 'O' && gameState[5] == 'O' && gameState[8] == 'O')
        return 2;
    else if(gameState[0] == 'X' && gameState[4] == 'X' && gameState[8] == 'X')
        return 1;
    else if(gameState[0] == 'O' && gameState[4] == 'O' && gameState[8] == 'O')
        return 2;
    else if(gameState[2] == 'X' && gameState[4] == 'X' && gameState[6] == 'X')
        return 1;
    else if(gameState[2] == 'O' && gameState[4] == 'O' && gameState[6] == 'O')
        return 2;
    else
        return 0;
}

int setMove(gData *p, char col, int row)
{
    int validMv = 1;

    memcpy(p->bcastMsg, "The game is afoot..\0", 21);
    
    /* Is move in range? */
    if(col > 'c')
        validMv = 0;
    if(row > 3)
        validMv = 0;

    /* If possible, set the move */
    if(col == 'a' && row == 1 && p->game[0] == '*')
        p->game[0] = p->playerTurn;
    else if(col == 'a' && row == 2 && p->game[3] == '*')
        p->game[3] = p->playerTurn;
    else if(col == 'a' && row == 3 && p->game[6] == '*')
        p->game[6] = p->playerTurn;
    else if(col == 'b' && row == 1 && p->game[1] == '*')
        p->game[1] = p->playerTurn;
    else if(col == 'b' && row == 2 && p->game[4] == '*')
        p->game[4] = p->playerTurn;
    else if(col == 'b' && row == 3 && p->game[7] == '*')
        p->game[7] = p->playerTurn;
    else if(col == 'c' && row == 1 && p->game[2] == '*')
        p->game[2] = p->playerTurn;
    else if(col == 'c' && row == 2 && p->game[5] == '*')
        p->game[5] = p->playerTurn;
    else if(col == 'c' && row == 3 && p->game[8] == '*')
        p->game[8] = p->playerTurn;
    else
        validMv = 0;

    return validMv;
}
