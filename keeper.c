#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <unistd.h>

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
    int sem;
}gInt;

struct gameArrayInt *initGame(char *shmName);
void parseMove(gInt *p);

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
            p->sem = 0; //Spurious

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
    memcpy(p->game, "*********\0", 10);
    printf("p->game = %s\n", p->game);

    //Initialise players to none
    p->playerX = '*';
    p->playerO = '*';
    p->playerTurn = 'X';
    memcpy(p->nextMove, "* \0", 3);
    p->sem = 1;
    return p;
}

void parseMove(gInt *p)
{
    char col = p->nextMove[0];
    int row = p->nextMove[1] - '0';
    printf(" -- parsing, new move is col %c, row %i\n", col, row);

    //Default to yes
    int validMv = 1;

    //Is move in range?
    if(col > 'c')
        validMv = 0;
    if(row > 3)
        validMv = 0;
        
    //Is move available?
    if(col == 'a' && row == 1 && p->game[0] == '*')
        p->game[0] = p->playerTurn;
    else if(col == 'a' && row == 2 && p->game[1] == '*')
        p->game[1] = p->playerTurn;
    else if(col == 'a' && row == 3 && p->game[2] == '*')
        p->game[2] = p->playerTurn;
    else if(col == 'b' && row == 1 && p->game[3] == '*')
        p->game[3] = p->playerTurn;
    else if(col == 'b' && row == 2 && p->game[4] == '*')
        p->game[4] = p->playerTurn;
    else if(col == 'b' && row == 3 && p->game[5] == '*')
        p->game[5] = p->playerTurn;
    else if(col == 'c' && row == 1 && p->game[6] == '*')
        p->game[6] = p->playerTurn;
    else if(col == 'c' && row == 2 && p->game[7] == '*')
        p->game[7] = p->playerTurn;
    else if(col == 'c' && row == 3 && p->game[8] == '*')
        p->game[8] = p->playerTurn;
    else
        validMv = 0;
        
    //Does this make a winner?


    //if move is valid
    if(validMv){

        //Switch turn
        if(p->playerTurn == 'X')
            p->playerTurn = 'O';
        else
            p->playerTurn = 'X';

        //Signal cont
        p->sem = 1;
    }else{
        //Illegal turn, do nothing, Signal cont
        p->sem = 1;
    }
}
