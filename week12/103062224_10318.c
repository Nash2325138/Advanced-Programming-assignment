#include <stdio.h>
#include <stdlib.h>
int row, col;
int pattern[3][3];
int pressed[6][6];
int lit[6][6];
int switchNum;
int isAnswered;
int ansBuffer[30];
int ansBufferSize;

void DFS(int cur_row, int cur_col)
{
    if( isAnswered ) return;
    if(cur_row == row) {
        /* examine is all lit ? */
        int i, j;
        for(i=cur_row-2 ; i<=cur_row-1 ; i++) {
            if(i < 0) continue;
            for(j=0 ; j<col ; j++) {
                if(lit[i][j] == 0) return;
            }
        }
        /* No return here means all grid are lit */
        isAnswered = 1;

        ansBufferSize = 0;
        for(i=0 ; i<row ; i++) {
            for(j=0 ; j<col ; j++) {
                /* printf("%d ", pressed[i][j]); */
                if(pressed[i][j]) ansBuffer[ansBufferSize++] = i*col+j+1;
            }
            /* printf("\n"); */
        }
        return;
    }
    if(cur_row >= 2) {
        /* if there's some grid is not lit two rows above, then there's no hope */
        int j;
        for(j=0 ; j<col ; j++) {
            if(lit[cur_row-2][j] == 0) {
                return;
            }
        }
    }
    /* not press */
    if(cur_col == col -1) {
        DFS(cur_row+1, 0);
    } else {
        DFS(cur_row, cur_col+1);
    }

    /* press */
    pressed[cur_row][cur_col] = 1;
    int i, j;
    int target_row, target_col;
    for(i=-1 ; i<=1 ; i++) {
        target_row = cur_row + i;
        if(target_row < 0 || target_row >= row) continue;
        for(j=-1 ; j<=1 ; j++) {
            target_col = cur_col + j;
            if(target_col < 0 || target_col >= col) continue;
            if(pattern[i+1][j+1] == 1) {
                lit[target_row][target_col] = !lit[target_row][target_col];
            }
        }
    }
    if(cur_col == col -1) {
        DFS(cur_row+1, 0);
    } else {
        DFS(cur_row, cur_col+1);
    }


    pressed[cur_row][cur_col] = 0;
    for(i=-1 ; i<=1 ; i++) {
        target_row = cur_row + i;
        if(target_row < 0 || target_row >= row) continue;
        for(j=-1 ; j<=1 ; j++) {
            target_col = cur_col + j;
            if(target_col < 0 || target_col >= col) continue;
            if(pattern[i+1][j+1] == 1) {
                lit[target_row][target_col] = !lit[target_row][target_col];
            }
        }
    }
}

void initial(int row, int col)
{
    int i, j;
    for(i=0 ; i<row ; i++) for(j=0 ; j<col ; j++){
        pressed[i][j] = 0;
        lit[i][j] = 0;
    }
    switchNum = isAnswered = 0;
}

int main()
{
    scanf("%d %d", &row, &col);
    int caseNum = 0;
    while( !(row==0 && col ==0) )
    {
        caseNum++;
        initial(row, col);
        int i, j;
        for(i=0 ; i<3 ; i++) {
            for(j=0 ; j<3 ; j++) {
                char symbol;
                scanf(" %c", &symbol);
                if(symbol == '*') {
                    pattern[i][j] = 1;
                    switchNum++;
                } else {
                    pattern[i][j] = 0;
                }
            }
        }
        DFS(0, 0);
        printf("Case #%d\n", caseNum);
        if(isAnswered) {
            printf("%d", ansBuffer[0]);
            for(i=1 ; i<ansBufferSize ; i++) printf(" %d", ansBuffer[i]);
            printf("\n");
        } else {
            printf("Impossible.\n");
        }
        scanf("%d %d", &row, &col);
    }
    return 0;
}
