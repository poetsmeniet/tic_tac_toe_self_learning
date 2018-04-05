#include <stdio.h>
#include <stdlib.h>
#define MAXBRANCHES 9 /* Array size */

/* Just a POC for generating an extensive gametree */

char currPlayer = 'X';

typedef struct tree{
    int nr;
    int points;
    char playerMove;
    struct tree *branche[MAXBRANCHES];
    struct tree *parent;
}tree;

int populateTree();

int traverseTree();
    
static int brancheCnt = 0;

int returnPoints(tree *branch, char currPlayer);

int findWinner();
    
int main(void)
{
    //int depth = 9; /* Nr of layers to branch */
    //int brCnt = 7; /* Nr of branches per node, dynamic.. */
    int depth = 9; /* Nr of layers to branch */
    int brCnt = 7; /* Nr of branches per node, dynamic.. */

    /* Check for sanity of the programmer */
    if(brCnt > MAXBRANCHES){
        printf("Nope, brCnt too high.. exiting\n");
        return 1;
    }

    /* Define binary tree */
    tree tree;
    tree.nr = 0;
    tree.points = 0;
    tree.playerMove = 's';
    tree.branche[0]= NULL;
    tree.parent = NULL;

    populateTree(&tree, depth, brCnt);

    printf("Root: %i\n", tree.nr);

    int i;
    int pathScores[depth];
    for(i = 1; i <= depth; i++)
        pathScores[i] = 0;

    traverseTree(&tree, 0, 0, &pathScores);
    
    printf("checking pathScores:\n");
    for(i = 1; i <= depth; i++){
        printf("path %i has total score of %i\n", i, pathScores[i]);
    }

    return 0;
}

/* Populate tree with arbitrary data */
int populateTree(tree *branch, int depth, int brCnt)
{
    /* Base case */
    if(depth == 0){
        printf("WE HAVE REACHED THE END: %i, points: %i\n", branch->nr, branch->points);
        return 0;
    }

    /* Supplied arbitrary value (id) for each node */
    static int cnt = 1;

    int j;
    char pm;

    /* Populate branch with data */
    for(j = 0; j < brCnt; j++){
        branch->branche[j] = malloc(sizeof(tree));
        branch->branche[j]->nr = cnt;

        /* determine next players move */
        if(branch->playerMove == 'X')
            pm = 'O';
        else
            pm = 'X';

        branch->branche[j]->playerMove = pm;
        branch->branche[j]->parent = branch;
        branch->branche[j]->branche[0]= NULL;
        branch->branche[j]->points = returnPoints(branch->branche[j], currPlayer);
        
        printf("Populated branch %d (parent: %d) with value %d\n", cnt,branch->branche[j]->parent->nr, branch->branche[j]->points);
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
int traverseTree(tree *tree, int parent, int points, int *pathScores)
{
    int j;

    int totPoints;
    static int nextMove = 0;

    /* Get the number of branches */
    int i = 1;
    while(tree->branche[i] != NULL)
        i++;

    /* Add gathered points */
    totPoints = points + tree->points;

    /* Go through this branch */
    for(j = 0; j < i; j++){
        /* Add score to path scores */
        if(tree->branche[j] == NULL){
            printf("\tStartstate cnt: %i, totPoints: %i\n", nextMove, totPoints);
            /* Add score of this path to array */
            pathScores[nextMove] += totPoints;
            return totPoints;
        }

        /* Increment path index */
        if(parent == 0)
            nextMove++;

        printf("nodenr: %i (parent: %i::%i), points: %i totPoints: %i.\tprev playerMove: %c, next playerMove: %c\n", \
                tree->branche[j]->nr, parent, tree->branche[j]->parent->nr, tree->branche[j]->points, totPoints, tree->branche[j]->parent->playerMove, tree->branche[j]->playerMove);
        
        traverseTree(tree->branche[j], tree->branche[j]->nr, totPoints, pathScores);
    }

    return 0;
}

/* Returns applicable nr of points for this branch */
int returnPoints(tree *branch, char currPlayer)
{
    char currPath[] = "*********";
    tree *curr = branch;



    /* Create gameState array */

    /* tmp: rewind to start of path */
    int i = -1;
    while(curr->playerMove != 's'){
        curr = curr->parent;
        i++;
    }
    
    curr = branch;
    while(curr->playerMove != 's'){
        currPath[i] = curr->playerMove;
        i--;
        curr = curr->parent;
    }
    printf("'%s' - ", currPath);

    /* EO Create gameState array */
    
    int winner = findWinner(currPath);

    printf("winnder: %i\n", winner);
    
    int points = 0;

    if(winner == 1)
        points = 1;
    if(winner == 2)
        points = -1;

    return points;
}

/*returns int:
 * 0: no winner (yet)
 * 1: X wins
 * 2: O wins */
int findWinner(char *gameState)
{
    if(gameState[0] == 'X' && gameState[1] == 'X' && gameState[2] == 'X')
        return 1;
    else if(gameState[0] == 'O' && gameState[1] == 'O' && gameState[2] == 'O')
       return 2;
    else if(gameState[3] == 'X' && gameState[4] == 'X' && gameState[5] == 'X')
        return 1;
    else if(gameState[3] == 'O' && gameState[4] == 'O' && gameState[5] == 'O')
        return 2;
    else if(gameState[6] == 'X' && gameState[7] == 'X' && gameState[8] == 'X')
        return 1;
    else if(gameState[6] == 'O' && gameState[7] == 'O' && gameState[8] == 'O')
        return 2;
    else if(gameState[0] == 'X' && gameState[3] == 'X' && gameState[6] == 'X')
        return 1;
    else if(gameState[0] == 'O' && gameState[3] == 'O' && gameState[6] == 'O')
        return 2;
    else if(gameState[1] == 'X' && gameState[4] == 'X' && gameState[7] == 'X')
        return 1;
    else if(gameState[1] == 'O' && gameState[4] == 'O' && gameState[7] == 'O')
        return 2;
    else if(gameState[2] == 'X' && gameState[5] == 'X' && gameState[8] == 'X')
        return 1;
    else if(gameState[2] == 'O' && gameState[5] == 'O' && gameState[8] == 'O')
        return 2;
    else if(gameState[0] == 'X' && gameState[4] == 'X' && gameState[8] == 'X')
        return 1;
    else if(gameState[0] == 'O' && gameState[4] == 'O' && gameState[8] == 'O')
        return 2;
    else if(gameState[2] == 'X' && gameState[4] == 'X' && gameState[6] == 'X')
        return 1;
    else if(gameState[2] == 'O' && gameState[4] == 'O' && gameState[6] == 'O')
        return 2;
    else
        return 0;
}
