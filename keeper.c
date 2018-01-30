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
}gInt;

struct gameArrayInt *initGame(char *shmName);

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
        printf("it is Player %c's turn\n\n", p->playerTurn);

        usleep(500000);
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
    return p;
}
