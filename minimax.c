#include <stdio.h>
#include <stdlib.h>

int isWinning(char *gameState, char currPlayer);

void returnAvailableMoves();
    
int main(void)
{
    /* Define some test variables */
    char gameState[] = "OOO**O**X";
    char currPlayer = 'X';
    printf("gamestate '%s', does player '%c' win: %d\n", gameState, currPlayer, isWinning(gameState, currPlayer));

    /* Return and loop available moves
     * - Run minimal for every avMove */
    int avMoves[8];
    int i = 0;
    returnAvailableMoves(gameState, avMoves);
    while(avMoves[i] != -1)
    {
        printf("avMove: %d\n", avMoves[i]); 
        i++;
    }

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
void returnAvailableMoves(char *gameState, int *avMoves)
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
}
