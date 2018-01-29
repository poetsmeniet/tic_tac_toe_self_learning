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
}gInt;

struct gameArrayInt *initGame(char *name);

int main(void)
{
    char name[] = "/gameState";

    struct gameArrayInt *p = initGame(name);
    sleep(5);

    memcpy(p->game, "*O*******\0", 10);
    printf("p->game = %s\n", p->game);

    sleep(5);

    munmap(&p, sizeof(int));

    shm_unlink(name);

    return 0;
}

/* Returns pointer to shared memory game state array */
struct gameArrayInt *initGame(char *name)
{
    int smfd = shm_open (name, O_RDWR | O_CREAT, 0660); 

    if(smfd == -1){
        perror("Unable to open shared memory\n");
        shm_unlink(name);
        return NULL;
    }

    struct gameArrayInt *p;
    p = mmap(NULL, sizeof(struct gameArrayInt), PROT_WRITE, MAP_SHARED, smfd, 0);

    if (ftruncate (smfd, sizeof (struct gameArrayInt)) == -1) 
        perror ("ftruncate");

    if(p < 0){
        perror("mmap failed.\n");
        munmap(&p, sizeof(int));
        return NULL;
    }

    //Init new gameState
    memcpy(p->game, "*********\0", 10);
    printf("p->game = %s\n", p->game);

    return p;
}
