#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <time.h> 
#include <stdlib.h> 
#include <unistd.h>
#include "tictactoe.h"
#define CLEAR() printf("\033[H\033[J") 

/* A quick and non optimized version of minimax for tictactoe */

void displayGame(gData *p, char player);

struct movesInfo{
    int index;
    int score;
};

/* Set whom is whom */
char aiPlayer;
char bioPlayer;

int isWinning(char *gameState, char currPlayer);

int returnAvailableMoves();

int miniMax(char *gameState, char player, struct movesInfo *moves);

int selectBestMove(struct movesInfo *moves);
    
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
    aiPlayer = player;
    if(aiPlayer == 'O')
        bioPlayer = 'X';
    else
        bioPlayer = 'O';

    /* Main loop */
    while(1){
        char nextMove[10];
        if(p->playerTurn == player && p->lck){
            displayGame(p, player);
            
            char gameState[10];
            memcpy(gameState, p->game, 9);
            gameState[9] = '\0';
            struct movesInfo moves[9];
            
            /* Truncate moves.. */
            int j;
            for(j = 0; j < 9; j++){
                moves[j].index = -1;
                moves[j].score = -1;
            }
            /* Run minimax */
            miniMax(gameState, aiPlayer, moves);
            
            /* Select best move */
            int bestMove = selectBestMove(moves);
            
            /* Convert move to human readable form */
            if(bestMove == 0)
                memcpy(nextMove, "a1", 2);
            else if(bestMove == 1)
                memcpy(nextMove, "b1", 2);
            else if(bestMove == 2)
                memcpy(nextMove, "c1", 2);
            else if(bestMove == 3)
                memcpy(nextMove, "a2", 2);
            else if(bestMove == 4)
                memcpy(nextMove, "b2", 2);
            else if(bestMove == 5)
                memcpy(nextMove, "c2", 2);
            else if(bestMove == 6)
                memcpy(nextMove, "a3", 2);
            else if(bestMove == 7)
                memcpy(nextMove, "b3", 2);
            else if(bestMove == 8)
                memcpy(nextMove, "c3", 2);

            /* Lock memory for use and set move to keeper */
            p->lck = 0; 
            memcpy(p->nextMove, nextMove, 3);
            p->nextMove[2] = '\0';

            /* Truncate moves.. */
            int i;
            for(i = 0; i < 9; i++){
                moves[i].index = -1;
                moves[i].score = -1;
            }

        }else{
            displayGame(p, player);
        }
        usleep(UDELAY);
    }

    munmap(&p, sizeof(gData));
    return 0;
}

/*returns int:
 * 0: no winner (yet)
 * 1: currPlayer wins (X | O)*/
int isWinning(char *gameState, char currPlayer)
{
    if((gameState[0] == currPlayer && gameState[1] == currPlayer && gameState[2] == currPlayer)\
    || (gameState[3] == currPlayer && gameState[4] == currPlayer && gameState[5] == currPlayer)\
    || (gameState[6] == currPlayer && gameState[7] == currPlayer && gameState[8] == currPlayer)\
    || (gameState[0] == currPlayer && gameState[3] == currPlayer && gameState[6] == currPlayer)\
    || (gameState[1] == currPlayer && gameState[4] == currPlayer && gameState[7] == currPlayer)\
    || (gameState[2] == currPlayer && gameState[5] == currPlayer && gameState[8] == currPlayer)\
    || (gameState[0] == currPlayer && gameState[4] == currPlayer && gameState[8] == currPlayer)\
    || (gameState[2] == currPlayer && gameState[4] == currPlayer && gameState[6] == currPlayer))
        return 1;
    else
        return 0;
}

/* Fills array of ints with indices of available moves */
int returnAvailableMoves(char *gameState, int *avMoves)
{
    int i;
    int j = 0;
    for(i = 0; i < 9; i++){
        if(gameState[i] == '*'){
            avMoves[j] = i;
            j++;
        }
    }

    avMoves[j] = -1;

    return j;
}

int miniMax(char *gameState, char player, struct movesInfo *moves)
{
    /* Return available moves */
    int avMoves[9];
    int i = 0;
    int avMvCnt = returnAvailableMoves(gameState, avMoves);
    
    /* Base case: return terminal gameState; win, lose, tie */
    if(isWinning(gameState, bioPlayer))
        return 1;
    else if(isWinning(gameState, aiPlayer))
        return -1;
    else if(avMvCnt == 0)
        return 0;
    else if(avMvCnt == 9){
        int rnd = rand() % 9;
        moves[avMoves[i]].index = rnd;
        moves[avMoves[i]].score = 1000;
        return 0;
    }
    
    /* Loop over all available moves */
    while(avMoves[i] != -1)
    {
        /* Store the index in movesInfo struct array */
        moves[avMoves[i]].index = avMoves[i];

        /* Make a move on gameState for curr player */
        gameState[avMoves[i]] = player;

        int result;

        /* Recurse minimax and save scores
         * In this version, scores are added or 
         * substracted, resulting in best move
         * with the highest score */
        if(player == aiPlayer){
            result = miniMax(gameState, bioPlayer, moves);
            moves[avMoves[i]].score -= result;
        }else{
            result = miniMax(gameState, aiPlayer, moves);
            moves[avMoves[i]].score += result;
        }

        /* Reset the move */
        gameState[avMoves[i]] = '*';
        
        i++;
    }
    
    return 0;
}

/* Returns the best move gamestate index */
int selectBestMove(struct movesInfo *moves)
{
    int i;
    int currBest = -1;
    int bestIndex;
    for(i = 0; i < 9; i++){
        if(moves[i].score >= currBest && moves[i].index != -1){
            bestIndex = moves[i].index;
            currBest = moves[i].score;
        }
    }

    return bestIndex;
}

void displayGame(gData *p, char player)
{
    CLEAR();
    printf("Minimax player: %c\n\n", player);

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
