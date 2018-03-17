#include <stdio.h>
#include <stdlib.h>
#define MAXBRANCHES 9 /* Array size */

/* Just a POC for generating an extensive gametree */

typedef struct tree{
    int nr;
    struct tree *branche[MAXBRANCHES];
}tree;

int populateTree();

int traverseTree();
    
static int brancheCnt = 0;
    
int main(void)
{
    //int depth = 9; /* Nr of layers to branch */
    //int brCnt = 9; /* Nr of branches per node, dynamic.. */
    int depth = 3; /* Nr of layers to branch */
    int brCnt = 3; /* Nr of branches per node, dynamic.. */

    /* Check for sanity of the programmer */
    if(brCnt > MAXBRANCHES){
        printf("Nope, brCnt too high.. exiting\n");
        return 1;
    }

    /* Define binary tree */
    tree tree;
    tree.nr = 0;
    tree.branche[0]= NULL;

    populateTree(&tree, depth, brCnt);

    printf("Root: %i\n", tree.nr);
    traverseTree(&tree, 0);
    printf("Branche count: %i\n", brancheCnt);
    
    return 0;
}

/* Populate tree with arbitrary data */
int populateTree(tree *branch, int depth, int brCnt)
{
    /* Base case */
    if(depth == 0)
        return 0;

    /* Supplied arbitrary value for tree branche */
    static int cnt = 1;

    int j;

    for(j = 0; j < brCnt; j++){
        branch->branche[j] = malloc(sizeof(tree));
        branch->branche[j]->nr = cnt;
        branch->branche[j]->branche[0]= NULL;
        
        brancheCnt++;
        cnt++;
    }

    /* Relying on last branch to be NULL */
    branch->branche[j]= NULL;

    depth--;
    brCnt--;

    for(j = 0; j <= brCnt; j++)
        populateTree(branch->branche[j], depth, brCnt);

    return 0;
}

/* Traverse the tree recursively */
int traverseTree(tree *tree, int parent)
{
    int j;

    //Get the number of branches
    int i = 0;
    while(tree->branche[i] != NULL)
        i++;

    /* Go through this branch */
    for(j = 0; j <= i; j++){
        /* Base case, exit on NULL pointer */
        if(tree->branche[j] == NULL)
            return 0;

        printf("nodenr: %i (parent: %i)\n", tree->branche[j]->nr, parent);
        
        traverseTree(tree->branche[j], tree->branche[j]->nr);
        
    }

    return 0;
}
