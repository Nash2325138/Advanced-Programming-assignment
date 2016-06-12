#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Relation{
    int head;
    int tail;
}Relation;

typedef struct AdjList{
    int adj;
    int degree;
}AdjList;

int relation_cmp(const void *a, const void *b)
{
    Relation *pa = (Relation *)a;
    Relation *pb = (Relation *)b;
    if(pa->head > pb->head) return 1;
    else if(pa->head < pb->head) return -1;
    else{
        return ( pa->tail > pb->tail ) - ( pa->tail < pb->tail);
    }
}
int DFS_countNode(int root);
Relation relation[1000010];
AdjList adjList[30010];
int visited[30010];

int main()
{
    int testCases;
    scanf("%d", &testCases);
    while(testCases--)
    {
        int citizens, pairs;
        scanf("%d %d", &citizens, &pairs);
        int i;

        /* construct relation */
        for(i=0 ; i<pairs*2 ; i+=2){
            scanf("%d %d", &relation[i].head, &relation[i].tail);
            relation[i+1].head = relation[i].tail;
            relation[i+1].tail = relation[i].head;
        }
        qsort(relation, pairs*2, sizeof(relation[0]), relation_cmp);

        /** should we consider when pairs==0 ?*/
        int maximum = 0;
        if(pairs==0) goto PRINT_ANS;

        /* construct adjList */
        int lastHead = relation[0].head;
        adjList[1].adj = adjList[1].degree = 0;
        int adjIter = 1;
        while(lastHead > adjIter){
            adjList[adjIter].adj = -1;
            adjIter++;
            adjList[adjIter].degree = 0;
        }
        adjList[adjIter].adj = 0;

        for(i=0 ; i<pairs*2 ; i++){
            /*printf("relation: %d %d\n", relation[i].head, relation[i].tail);*/
            if(lastHead == relation[i].head){
                adjList[adjIter].degree++;
            }
            else{
                assert(lastHead < relation[i].head);
                lastHead = relation[i].head;
                adjIter++;
                while(lastHead > adjIter){
                    adjList[adjIter].adj = -1;
                    adjIter++;
                    adjList[adjIter].degree = 0;
                }
                adjList[adjIter].adj = i;
                adjList[adjIter].degree = 1;
            }
            /*printf("adjList[%d].adj==%d, adjList[%d].degree==%d\n\n", adjIter, adjList[adjIter].adj, adjIter, adjList[adjIter].degree);*/
        }
        adjIter++;
        while(adjIter <= citizens){
            adjList[adjIter].adj = -1;
            adjList[adjIter].degree = 0;
            adjIter++;
        }
        assert(adjIter == citizens + 1);

        /*for(i=1 ; i<=citizens ; i++){
            printf("%d %d\n", adjList[i].adj, adjList[i].degree);
        }*/

        memset(visited, 0, sizeof(visited));
        for(i=1 ; i<=citizens ; i++){
            if(visited[i]) continue;
            int result = DFS_countNode(i);
            if(result > maximum) maximum = result;
        }

        PRINT_ANS:;
        printf("%d\n", maximum);

    }
    return 0;
}

int DFS_countNode(int root)
{
    int start = adjList[root].adj;
    int num = adjList[root].degree;
    if(visited[root]==1) return 0;
    if(start==-1){
        return 1;
    }
    visited[root] = 1;

    int sum = 1;
    int i;
    for(i=0 ; i<num ; i++){
        sum += DFS_countNode(relation[start+i].tail);
    }
    return sum;
}
