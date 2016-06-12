#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct State{
    /*int isFromEnd;*/
    int step;
    char color[5][5];
}State;
const int dir[8][2] = {{-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}};

State *used[524300];
int usedSize;

/* Q is supposed to be big enough to be not circular */
State *Qstart[262150];
int startHead, startTail;

State *Qend[262150];
int endHead, endTail;

void initial_read()
{
    startHead = startTail = endHead = endTail = 0;

        int i, j;
        State *next;
        next = (State *)malloc(sizeof(State));
        next->step = 0;
        /*next->isFromEnd = 0;*/
        for(i=0 ; i<5 ; i++) {
            for(j=0 ; j<5 ; j++) {
                next->color[i][j] = getchar();
            }
            getchar();
        }
        Qstart[startTail++] = next;

        next = (State *)malloc(sizeof(State));
        next->step = 0;
        /*next->isFromEnd = 1;*/
        for(i=0 ; i<5 ; i++) {
            for(j=0 ; j<5 ; j++) {
                if(i>j) next->color[i][j] = '0';
                else if(j>i) next->color[i][j] = '1';
                else {
                    if(i<2) next->color[i][j] = '1';
                    else if(i>2) next->color[i][j] = '0';
                    else next->color[i][j] = ' ';
                }
            }
        }
        Qend[endTail++] = next;

}
int compare(State *s1, State *s2)
{
    int i, j;
    for(i=0 ; i<5; i++) {
        for(j=0 ; j<5 ; j++) {
            if(s1->color[i][j] != s2->color[i][j]) return 0;
        }
    }
    return 1;
}
void find_empty(State *s, int *empty_row, int *empty_col)
{
    int i, j;
    for(i=0 ; i<5 ; i++) {
        for(j=0 ; j<5 ; j++) {
            if(s->color[i][j] == ' ') {
                *empty_row = i;
                *empty_col = j;
                return;
            }
        }
    }
}
void print_state(State *s)
{
    printf("step: %d\n", s->step);
    int i, j;
    for(i=0 ; i<5 ; i++) {
        for(j=0 ; j<5 ; j++) {
            printf("%c", s->color[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    int testCases;
    scanf("%d ", &testCases);
    while(testCases--)
    {
        initial_read();
        int ans = -1;
        for( ; ; )
        {
            State *now;
            State *next;
            int i;
            int currentStep;

            /* from start */
            do {
                if(startTail == startHead) break; /** which is impossible in my implementation **/
                now = Qstart[startHead++];
                /*printf("Start get  ");
                print_state(now);*/

                currentStep = now->step;
                for(i=endHead ; i<endTail ; i++) {
                    if(compare(now, Qend[i]) == 1) {
                        ans = now->step + Qend[i]->step;
                        break;
                    }
                }
                if(ans != -1) break;
                int empty_row, empty_col;
                find_empty(now, &empty_row, &empty_col);
                for(i=0 ; i<8 ; i++){
                    int next_row = empty_row + dir[i][0];
                    int next_col = empty_col + dir[i][1];
                    if( next_row < 0 || next_row > 4 ) continue;
                    if( next_col < 0 || next_col > 4 ) continue;

                    next = (State *)malloc(sizeof(State));
                    memcpy(next, now, sizeof(State));
                    next->step++;
                    next->color[empty_row][empty_col] = next->color[next_row][next_col];
                    next->color[next_row][next_col] = ' ';
                    Qstart[startTail++] = next;
                }
            }while(Qstart[startHead]->step == currentStep);
            if(ans != -1) break;
            if(currentStep == 5) break;

            /* from end */
            do {
                if(endTail == endHead) break; /** which is impossible in my implementation **/
                now = Qend[endHead++];
                /*printf("End get  ");
                print_state(now);*/
                currentStep = now->step;
                for(i=startHead ; i<startTail ; i++) {
                    if(compare(now, Qstart[i]) == 1) {
                        ans = now->step + Qstart[i]->step;
                        break;
                    }
                }
                if(ans != -1) break;

                int empty_row, empty_col;
                find_empty(now, &empty_row, &empty_col);
                for(i=0 ; i<8 ; i++){
                    int next_row = empty_row + dir[i][0];
                    int next_col = empty_col + dir[i][1];
                    if( next_row < 0 || next_row > 4 ) continue;
                    if( next_col < 0 || next_col > 4 ) continue;

                    next = (State *)malloc(sizeof(State));
                    memcpy(next, now, sizeof(State));
                    next->step++;
                    next->color[empty_row][empty_col] = next->color[next_row][next_col];
                    next->color[next_row][next_col] = ' ';
                    Qend[endTail++] = next;
                }
            }while(Qend[endHead]->step == currentStep);
            if(ans != -1) break;

        }
        if(ans == -1) printf("Unsolvable in less than 11 move(s).\n");
        else printf("Solvable in %d move(s).\n", ans);


    }
    return 0;
}
