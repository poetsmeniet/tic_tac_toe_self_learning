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
    int smfd = shm_open (name, O_RDWR | O_CREAT, 0660); 

    if(smfd == -1){
        perror("Unable to open shared memory\n");
        shm_unlink(name);
        return 1;
    }

    struct gameArrayInt *p;
    p = mmap(NULL, sizeof(struct gameArrayInt), PROT_WRITE, MAP_SHARED, smfd, 0);

    if (ftruncate (smfd, sizeof (struct gameArrayInt)) == -1) 
        perror ("ftruncate");

    if(p == -1){
        perror("mmap failed.\n");
        munmap(&p, sizeof(int));
        return 1;
    }

    //test
    p->game = 13;
    printf("p->game = %i\n", p->game);
    
    sleep(5);

    p->game = 135;
    printf("p->game = %i\n", p->game);

    sleep(5);

    munmap(&p, sizeof(int));

    shm_unlink(name);

    return 0;
}
