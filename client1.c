#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <unistd.h>

typedef struct gameArrayInt{
    int game;
}gInt;

int main(void)
{
    char name[] = "/sharedmem1";
    int smfd = shm_open (name, O_RDONLY, 0660); 

    if(smfd == -1){
        perror("Unable to open shared memory\n");
        return 1;
    }

    struct gameArrayInt *p;
    p = mmap(NULL, sizeof(struct gameArrayInt), PROT_READ, MAP_SHARED, smfd, 0);

    if(p == -1){
        perror("mmap failed.\n");
        return 1;
    }

    printf("p->game read from master process = %i\n", p->game);

    return 0;
}
