#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <unistd.h>
#include "tictactoe.h"

struct gameArrayInt *initGame(char *shmName);
int parseMove(gInt *p);
void newGame(gInt *p);
int findWinner(char *gameState);
int setMove(gInt *p, char col, int row);

int main(void)
{
    char shmName[] = "/gameState";

    struct gameArrayInt *p = initGame(shmName);

    //parse moves
    while(1){
        //temporarily use usleep in loop
        printf("gameState: %s\n", p->game);
        printf("PlayerX: %c\n", p->playerX);
        printf("PlayerO: %c\n", p->playerO);
        printf("it is Player %c's turn\n", p->playerTurn);
        printf("nextMove: %s\n\n", p->nextMove);

        if(p->nextMove[0] != '*'){
            //Signal stop to clients
            p->lck = 0; //Spurious

            printf("-parse move-\n");
            parseMove(p);
            memcpy(p->nextMove, "* \0", 3);
        }

        //usleep(500000);
        usleep(50000);
    }

    munmap(&p, sizeof(struct gameArrayInt));

    shm_unlink(shmName);

    return 0;
}

/* Returns pointer to shared memory game state array */
struct gameArrayInt *initGame(char *shmName)
{
    int smfd = shm_open (shmName, O_RDWR | O_CREAT, 0660); 

    if(smfd == -1){
        perror("Unable to open shared memory\n");
        shm_unlink(shmName);
        return NULL;
    }

    struct gameArrayInt *p;
    p = mmap(NULL, sizeof(struct gameArrayInt), PROT_WRITE, MAP_SHARED, smfd, 0);

    if (ftruncate (smfd, sizeof (struct gameArrayInt)) == -1) 
        perror ("ftruncate");

    if(p < 0){
        perror("mmap failed.\n");
        munmap(&p, sizeof(struct gameArrayInt));
        return NULL;
    }

    //Init new gameState
    newGame(p);

    return p;
}

int parseMove(gInt *p)
{
    char col = p->nextMove[0];
    int row = p->nextMove[1] - '0';

    if(setMove(p, col, row) == 0){
        printf("setmove returns 0\n");
        sleep(5);
        p->lck = 1;
        return 0;
    }
    
    //Calculate draw
    int i;
    int e;

    for(i = 0; i < 10; i++){
        if(p->game[i] == '*')
            e++;
    }

    if(e == 0){
        printf("Draw..\n");
        sleep(5);
        newGame(p);
        return 0;
    }

    //Find a winner
    if(findWinner(p->game) == 1){
        printf("X WINs, print some more stuff and start new game\n");
        memcpy(p->bcastMsg, "X wins this game! Starting new game..\0", 38);
        sleep(5);
        newGame(p);
        return 0;
    }else if(findWinner(p->game) == 2){
        printf("O WINs, print some more stuff and start new game\n");
        memcpy(p->bcastMsg, "O wins this game! Starting new game..\0", 38);
        sleep(5);
        newGame(p);
        return 0;
    }
    
    //Switch turn
    if(p->playerTurn == 'X')
        p->playerTurn = 'O';
    else
        p->playerTurn = 'X';

    p->lck = 1;

    return 0;
}

void newGame(gInt *p)
{
    //Init new gameState
    memcpy(p->game, "*********\0", 10);
    printf("p->game = %s\n", p->game);

    //Initialise players to none
    p->playerX = '*';
    p->playerO = '*';
    p->playerTurn = 'X';
    memcpy(p->nextMove, "* \0", 3);
    memcpy(p->bcastMsg, "Welcome to a new game\0", 22);
    p->lck = 1;
}

/*returns int:
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

int setMove(gInt *p, char col, int row)
{
    int validMv = 1;

    memcpy(p->bcastMsg, "The game is afoot..\0", 21);
    
    //Is move in range?
    if(col > 'c')
        validMv = 0;
    if(row > 3)
        validMv = 0;

    //If possible, set the move
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
