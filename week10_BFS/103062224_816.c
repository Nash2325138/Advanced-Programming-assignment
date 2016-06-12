#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EAST 0
#define SOUTH 1
#define WEST 2
#define NORTH 3
typedef struct Node
{
    /* duplicated information for maze array */
    int row, col, direction;

    /* information that won't change after reading the input */
    int linked[4];

    /* information that will be modified during BFS */
    int visited;
    struct Node *parent;
}Node;

Node maze[10][10][4];
Node* que[500];
int Qhead, Qtail;

void initialize_maze(void)
{
    int i, j, k;
    for(i=0 ; i<10 ; i++) for(j=0 ; j<10 ; j++) for(k=0 ; k<4 ; k++){
        maze[i][j][k].row = i;
        maze[i][j][k].col = j;
        maze[i][j][k].direction = k;

        maze[i][j][k].parent = NULL;
        maze[i][j][k].visited = 0;
        maze[i][j][k].linked[0] = maze[i][j][k].linked[1] = maze[i][j][k].linked[2] = maze[i][j][k].linked[3] = 0;
    }
}
char itoc(int i)
{
    if(i==EAST) return 'E';
    if(i==SOUTH) return 'S';
    if(i==WEST) return 'W';
    if(i==NORTH) return 'N';
    return 'X';
}
void print_maze(void)
{
    int i, j, k;
    for(i=1 ; i<10 ; i++) for(j=1 ; j<10 ; j++) for(k=0 ; k<4 ; k++){
        printf("(%d,%d,%c): ", i, j, itoc(k));
        int l;
        for(l=0 ; l<4 ; l++) if(maze[i][j][k].linked[l]) printf("%c ", itoc(l));
        printf("\n");
    }
}
void printQ(void)
{
    int i;
    for(i=Qhead ; i<Qtail ; i++) printf("(%d,%d,%c) ", que[i]->row, que[i]->col, itoc(que[i]->direction) );
    printf("\n");
}
void read_maze(void)
{
    char tempC;
    int input_row, input_col, input_direction;
    scanf("%d", &input_row);
    while(input_row != 0)
    {
        scanf("%d", &input_col);
        scanf(" %c", &tempC);
        while(tempC != '*')
        {
            if(tempC=='E') input_direction = WEST;
            else if(tempC=='S') input_direction = NORTH;
            else if(tempC=='W') input_direction = EAST;
            else if(tempC=='N') input_direction = SOUTH;
            else fprintf(stderr, "no such input direction : %c\n", tempC);

            while( (tempC = getchar()) != ' ' )
            {
                if(tempC=='L') maze[input_row][input_col][input_direction].linked[ (input_direction+1)%4 ] = 1;
                else if(tempC=='F') maze[input_row][input_col][input_direction].linked[ (input_direction+2)%4 ] = 1;
                else if(tempC=='R') maze[input_row][input_col][input_direction].linked[ (input_direction+3)%4 ] = 1;
                else fprintf(stderr, "no such Left or Right or Forward : %c\n", tempC);
            }

            scanf(" %c", &tempC);
        }
        scanf("%d", &input_row);
    }
}
void pushQ(Node *parent, Node *next)
{
    if(next->visited) return;
    if(next->parent != NULL) return;
    next->parent = parent;
    que[Qtail] = next;
    Qtail++;
}
Node *popQ(void)
{
    que[Qhead]->visited = 1;
    Qhead++;
    return que[Qhead-1];
}
void BFS(int end_row, int end_col)
{
    while(Qhead < Qtail)
    {
        /* printQ(); */
        Node *current = popQ();
        if(current->row == end_row && current->col == end_col) return;
        int i=0;
        int array[4] = {NORTH, WEST, EAST, SOUTH};
        for(i=0 ; i<4 ; i++){
            if(current->linked[array[i]] == 1){
                if(array[i]==EAST){
                    pushQ(current, &maze[current->row][current->col + 1][WEST]);
                } else if(array[i]==SOUTH){
                    pushQ(current, &maze[current->row + 1][current->col][NORTH]);
                } else if(array[i]==WEST){
                    pushQ(current, &maze[current->row][current->col - 1][EAST]);
                } else if(array[i]==NORTH) {
                    pushQ(current, &maze[current->row - 1][current->col][SOUTH]);
                } else fprintf(stderr, "WTF?\n");
            }
        }
    }
    /* printf("Q runs out!\n"); */
}

int main()
{
    /*freopen("816.in", "r", stdin);
    freopen("my.out", "w", stdout);*/
    char name[30];
    char tempC;

    scanf("%s", name);
    while(strcmp(name, "END")!=0)
    {
        initialize_maze();
        Qhead = Qtail = 0;

        int start_row, start_col, start_direction;
        int end_row, end_col;
        scanf("%d %d %c", &start_row, &start_col, &tempC);

        int start_row2 = start_row, start_col2 = start_col, start_direction2;
        /* translate input (starting destination) into my meaning of node in que(starting source) */
        if(tempC=='E') {
            start_col2++;
            start_direction2 = WEST;
        } else if(tempC=='S') {
            start_row2++;
            start_direction2 = NORTH;
        } else if(tempC=='W') {
            start_col2--;
            start_direction2 = EAST;
        } else if(tempC=='N') {
            start_row2--;
            start_direction2 = SOUTH;
        } else fprintf(stderr, "no such start direction : %c\n", tempC);
        pushQ(NULL, &maze[start_row2][start_col2][start_direction2]);

        scanf("%d %d", &end_row, &end_col);
        read_maze();
        /* print_maze(); */

        BFS(end_row, end_col);
        /* if maze[end_row][end_col][0~3].visited are all 0 */
        int i;
        Node *current = NULL;
        for(i=0 ; i<4 ; i++){
            if( maze[end_row][end_col][i].visited ){
                current = &maze[end_row][end_col][i];
            }
        }

        printf("%s\n", name);
        if(current==NULL){
            printf("  No Solution Possible");
        }
        else
        {
            Node * Nstack[500];
            int NstackSize = 0;
            Nstack[NstackSize++] = current;
            while(current->parent != NULL){
                current = current->parent;
                Nstack[NstackSize++] = current;
            }
            int count = 1;
            i = NstackSize-1;
            printf("  (%d,%d)", start_row, start_col);
            if(i >= 0) putchar(' ');
            while(i >= 0)
            {
                count++;
                printf("(%d,%d)", Nstack[i]->row, Nstack[i]->col);
                if(i!=0){
                    if(count%10 == 0) printf("\n  ");
                    else putchar(' ');
                }
                i--;
            }
        }
        putchar('\n');

        scanf("%s", name);
    }

    return 0;
}
