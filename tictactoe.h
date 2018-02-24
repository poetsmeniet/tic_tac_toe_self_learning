#ifndef tictactoe_H_
#define tictactoe_H_

/* The game state:
 * * = empty 
 * O = Player O position
 * X = Player X position
 */
typedef struct gameData{
    char game[10];
    char playerX;
    char playerO;
    char playerTurn;
    char nextMove[3];
    int lck; //lck == 1: continue, else locked
    char bcastMsg[200];
    char winner;
}gData;

#endif
