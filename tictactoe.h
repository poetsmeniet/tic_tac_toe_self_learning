#ifndef tictactoe_H_
#define tictactoe_H_

//#define UDELAY 100000 /* Delay between moves, humans may follow */
//#define UDELAY2 2000000 /* Delay between games,  humans may follow */

#define UDELAY 5000 /* Delay between moves, ai speed for testing */
#define UDELAY2 1000 /* Delay between games,  ai speed for testing */

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
