#ifndef tictactoe_H_
#define tictactoe_H_

//#define UDELAY 1000
#define UDELAY 10000

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
    int scoreX;
    int scoreO;
    int ties;
}gData;

#endif
