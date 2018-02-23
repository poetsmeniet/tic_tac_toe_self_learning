#ifndef tictactoe_H_
#define tictactoe_H_

/* The game state:
 * * = empty 
 * O = Player O position
 * X = Player X position
 */
typedef struct gameArrayInt{
    char game[10];
    char playerX;
    char playerO;
    char playerTurn;
    char nextMove[3];
    int lck;
    char bcastMsg[200];
}gInt;

#endif
