#include <stdio.h>
#include <stdlib.h>

int isWinning(char *gameState, char currPlayer);

int returnAvailableMoves();

int miniMax(char *gameState, char aiPlayer, char bioPlayer);
    
int main(void)
{
    /* Define some test variables */
    char gameState[] = "OOO**O**X";
    
    /* Set whom is whom */
    char aiPlayer = 'X';
    char bioPlayer = 'O';
    
    int test[8];
    printf("test: %d\n", returnAvailableMoves(gameState, test));
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

int miniMax(char *gameState, char aiPlayer, char bioPlayer)
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
        printf("avMove: %d\n", avMoves[i]); 
        i++;
    }

}
