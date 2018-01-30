#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <unistd.h>

typedef struct gameArrayInt{
    char game[10];
    char playerX;
    char playerO;
    char playerTurn;
}gInt;

int main(void)
{
    char name[] = "/gameState";
    int smfd = shm_open (name, O_RDWR, 0660); 

    if(smfd == -1){
        perror("Unable to open shared memory\n");
        return 1;
    }

    struct gameArrayInt *p;
    p = mmap(NULL, sizeof(struct gameArrayInt), PROT_WRITE, MAP_SHARED, smfd, 0);

    if(p < 0){
        perror("mmap failed.\n");
        return 1;
    }

    printf("p->game read from master process = %s\n", p->game);

    char player;
    printf("Trying to be player X..\n");
    if(p->playerX == '*'){
        p->playerX = 'X';
        player = 'X';
    }else if(p->playerO == '*'){
        p->playerO = 'O';
        player = 'O';
    }else{
        printf("No available player slots\n");
        return 1;
    }
    printf("This instance is player %c\n", player);

    /* Main loop */
    while(1){
        printf("is %c == %c?\n", p->playerTurn, player);
        //Turn?
        if(p->playerTurn == player){
            printf("Please make a move:\n?>");
            sleep(10);
        }
        sleep(1);
    }

    munmap(&p, sizeof(struct gameArrayInt));
    return 0;
}
