#include <stdio.h>
#include <stdlib.h>

struct movesInfo{
    int index;
    int score;
};

/* Set whom is whom */
char aiPlayer = 'X';
char bioPlayer = 'O';

int isWinning(char *gameState, char currPlayer);

int returnAvailableMoves();

int miniMax(char *gameState, char player, struct movesInfo *moves);

int selectBestMove(struct movesInfo *moves);
    
int main(void)
{
    /* Define some test variables */
    char gameState[] = "XOX**O**X";
    struct movesInfo moves[1000000];
    
    /* Run minimax */
    miniMax(gameState, aiPlayer, moves);
    
    /* Select best move */
    int bestMove = selectBestMove(moves);
    printf("Index chosen for best move: %d\n", bestMove);
    
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
    int avMoves[8];
    int i = 0;
    int avMvCnt = returnAvailableMoves(gameState, avMoves);
    
    /* Base case: return terminal gameState; win, lose, tie */
    if(isWinning(gameState, aiPlayer))
        return 1;
    else if(isWinning(gameState, bioPlayer))
        return -1;
    else if(avMvCnt == 0)
        return 0;
    
    /* Loop over all available moves */
    while(avMoves[i] != -1)
    {
        //printf("avMove: %d\n", avMoves[i]); 
        
        /* Store the index in movesInfo struct array */
        moves[avMoves[i]].index = avMoves[i];

        /* Make a move on gameState for curr player */
        gameState[avMoves[i]] = player;

        int result;

        //Recurse minimax
        if(player == aiPlayer)
            result = miniMax(gameState, bioPlayer, moves);
        else
            result = miniMax(gameState, aiPlayer, moves);

        /* Save score to struct */
        moves[avMoves[i]].score += result;

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
    int currBest = 0;
    int bestIndex;
    for(i = 0; i < 9; i++){
        //printf("\tA score of %d is found, with index: %d\n", moves[i].score, moves[i].index);
        if(moves[i].score > currBest){
            //printf("\t\tSetting highest score of %d is found, with index: %d\n", moves[i].score, moves[i].index);
            bestIndex = moves[i].index;
            currBest = moves[i].score;
        }
    }
    return bestIndex;
}
